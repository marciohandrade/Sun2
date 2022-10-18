# [Prerequisites]
# Visual Studio, Subversion, Perforce, 7-Zip이 설치되어 있을 것
# [svn_root]/SUN/trunk에 매핑된 로컬 working copy가 준비되어 있을 것
# Perforce ClientSpec이 적절히 설정되어 있을 것
# devenv.exe, svn.exe, 7z.exe에 대한 Path가 잡혀 있을 것

require 'fileutils'
require 'ftools'
require 'getoptlong'

require 'incredibuild.rb'
require 'msdev.rb'
require 'svnclient.rb'
require 'perforce.rb'
require 'date'

# global variables for command-line arguments
username = '' # subversion username
password = '' # subversion password
quiet = false # whether to enable console interaction (true/false)
no_update = false # whether to skip updating working copy to HEAD revision
buildgame = false # whether to build game server group also (true/false)
buildauth = false # whether to build auth server group also (true/false)
rebuild = false # whether to perform full rebuild (true/false)
gamelib = false # whether to include game logic library release (true/false) 
loglib = false # whether to include game log library release (true/false)
p4root = 'D:\Perforce' # perforce clientspec root local directory
target_root = 'D:\Release' # release target root directory
use_incredibuild = false

url = '' # reposotiry source URL

path = '../..'
target_country = ''
version = 0
hackshield_version = 0
xigncode_version = 0
prev_build_num = -1
curr_build_num = -1
solution_file='Server/SUNServer.sln'

# Multiple anti-hack tool support
class AntiHackTool
  NONE = 0
  HACKSHIELD = 1
  FROST = 2 # Russia
  XIGNCODE = 3
end
anti_hack_tool = AntiHackTool::NONE

opts = GetoptLong.new(
  [ '--username', '-u', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--password', '-p', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--quiet', '-q', GetoptLong::NO_ARGUMENT ],
  [ '--no-update', '-n', GetoptLong::NO_ARGUMENT ],
  [ '--buildgame', '-b', GetoptLong::NO_ARGUMENT ],
  [ '--buildauth', '-a', GetoptLong::NO_ARGUMENT ],
  [ '--rebuild', '-r', GetoptLong::NO_ARGUMENT ],
  [ '--gamelib', '-g', GetoptLong::NO_ARGUMENT ],
  [ '--loglib', '-l', GetoptLong::NO_ARGUMENT ],
  [ '--p4root', '-c', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--target', '-t', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--use-incredibuild', '-i', GetoptLong::NO_ARGUMENT ]
)

opts.each do |opt, arg|
  case opt
    when '--username' then username = arg
    when '--password' then password = arg
    when '--quiet' then quiet = true
    when '--no-update' then no_update = true
    when '--buildgame' then buildgame = true
    when '--buildauth' then buildauth = true
    when '--rebuild' then rebuild = true
    when '--gamelib' then gamelib = true
    when '--loglib' then loglib = true
    when '--p4root' then p4root = arg
    when '--target' then target_root = arg
    when '--use-incredibuild' then use_incredibuild = true
  end
end

if not (buildgame or buildauth or gamelib or loglib)
  puts "[OK] Nothing to do"
  exit(1)
end

if ARGV.length < 1
  puts "[ERROR] No repository URL specified"
  exit(-1)
end

today = Date.today.strftime("%y%m%d") # date string of format "yymmdd"

url = ARGV[0]

svn = SvnClient.new
svn.username = username
svn.password = password
svn.quiet = quiet

if use_incredibuild == true
  devenv = IncrediDevEnv.new
else
  devenv = DevEnv.new
end
devenv.quiet = quiet

p4 = P4Client.new
p4.bin_path = '..\_bin\util'
p4.quiet = quiet

unless no_update
  if buildgame or buildauth
    uncommited_local_changes_allowed = false
    unless uncommited_local_changes_allowed 
      # Should have no local modifications
      if svn.check_for_modifications(path)
        puts "[ERROR] Local modification must be commited or reverted before release"
        exit(-1)
      end
    end
    # Update working copy
    if svn.switch(url, path) == false
      puts "[ERROR] Couldn't switch to \"#{url}\" on \"#{File.expand_path(path)}\""
      exit(svn.last_error)
    end
  end
end

# Identify target country setting
IO.foreach(File.join(path, 'ProgramCommon/CountryCode.h')) do |line|
  s = line.strip
  next if s[0..1] == '//' or s == ''
  tokens = s.split
  got_it = false
  tokens.each do |token|
    if got_it
      target_country = token
      break
    else
      got_it = true if token.include?('define')
    end 
  end
end

if target_country == ''
  puts "[ERROR] No target country specification found"
  exit(-1)
end

# Country-specific settings
country_ver_prefix = 0
country_abbrev = ''
logic_target = ''
tag_target = ''
case target_country
  when '_KOREA'
    country_ver_prefix = 1
    country_abbrev = 'kr'
    logic_target = 'KO'
    tag_target = 'kor'
    anti_hack_tool = AntiHackTool::XIGNCODE
  when '_JAPAN'
    country_ver_prefix = 2
    country_abbrev = 'jp'
    logic_target = 'JP'
    tag_target = 'jp'
    anti_hack_tool = AntiHackTool::XIGNCODE
  when '_USA'
    country_ver_prefix = 3
    country_abbrev = 'us'
    logic_target = 'EN'
    tag_target = 'us'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_TAIWAN'
    country_ver_prefix = 4
    country_abbrev = 'tw'
    logic_target = 'TW'
    tag_target = 'tw'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_CHINA'
    country_ver_prefix = 5
    country_abbrev = 'cn'
    logic_target = 'CN'
    tag_target = 'cn'
    anti_hack_tool = AntiHackTool::XIGNCODE
  when '_GLOBAL'
    country_ver_prefix = 7
    country_abbrev = 'gp'
    logic_target = 'GLOBAL'
    tag_target = 'gsp'
    anti_hack_tool = AntiHackTool::XIGNCODE
  when '_RUSSIA'
    country_ver_prefix = 8
    country_abbrev = 'ru'
    logic_target = 'RU'
    tag_target = 'ru'
    anti_hack_tool = AntiHackTool::FROST
end

# Identify version info
in_search = false
IO.foreach(File.join(path, 'ProgramCommon/Version.h')) do |line|
  s = line.strip
  next if s[0..1] == '//' or s == ''
  if s.include?('#') and s.include?('if') and s.include?('defined') # if defined( _COUNTRY )
     in_search = s.include?(target_country)
  else
    if in_search and ( s.include?('define') and ( s.include?('NATION_BUILD_VERSION') or s.include?('HACKSHIELD_VERSION') or s.include?('XIGNCODE_VERSION') ) )
      tokens = s.split
      item = ''
      tokens.each do |token|
        case item
          when ''
            item = 'define' if token.include?('define')
          when 'define'
            case token
              when 'NATION_BUILD_VERSION' then item = token
              when 'HACKSHIELD_VERSION' then item = token
              when 'XIGNCODE_VERSION' then item = token
            end
          else
            case item
              when 'NATION_BUILD_VERSION'
                version = token.to_i
                break
              when 'HACKSHIELD_VERSION'
                hackshield_version = token.to_i
                break
              when 'XIGNCODE_VERSION'
                xigncode_version = token.to_i
                break
            end
        end
      end
    end
  end
end

if version == 0
  puts "[ERROR] No target version specification found"
  exit(-1)
end

if anti_hack_tool == AntiHackTool::HACKSHIELD and hackshield_version == 0
  puts "[ERROR] No Hackshield version specification found"
  exit(-1)
end

version_str = "%04d" % version
version_str.insert(3, '.')
version_str.insert(2, '.')
version_str.insert(1, '.')

# Identiry build number and increase it if required
file_base = File.join(path, 'Server/Common/ServerBuildVersion.cpp')
if no_update
  svn.update(file_base)
end
temp_file = File.new(file_base + '.new', 'w+')
if temp_file
  in_search = false
  IO.foreach(file_base) do |line|
    s = line
    if not in_search and s.include?('#') and s.include?('if') and s.include?('def') and s.include?(target_country)
      in_search = true
    else
      if in_search and s.strip.length > 0
        if s.include?('buildNumber')
          tokens = s.tr('=;', ' ').split
          if tokens.length > 1 then
            prev_build_num = tokens[1].to_i
            curr_build_num = prev_build_num
            curr_build_num = curr_build_num + 1 if buildgame or buildauth
            line.sub!(tokens[1], curr_build_num.to_s)
          end
        end
        in_search = false
      end
    end
    temp_file.write(line)
  end
  temp_file.close
  
  if prev_build_num == -1
    File.delete(file_base + '.new')
    puts "[ERROR] Failed to configure the build number"
    exit(-1)
  end

  if buildgame or buildauth
    File.delete(file_base)
    File.rename(file_base + '.new', file_base)
  else
    File.delete(file_base + '.new')
  end
else
  puts "[ERROR] Couldn't open temp file for ServerBuildVersion.cpp"
  exit(-1)
end

full_version = 0
full_version = full_version + curr_build_num
full_version = full_version + (version * 100)
full_version = full_version + (country_ver_prefix * 1000000)

release_target = File.join(target_root, country_abbrev)
release_target = File.join(release_target, "SUNServer_#{country_abbrev}_#{full_version}_#{today}")

File.makedirs(release_target)

unless quiet  
  # Require user confirm
  puts "Release settings :"
  puts "      Repository URL = #{url}"
  puts "  Build Game Servers = #{buildgame}"
  puts "  Build Auth Servers = #{buildauth}"
  puts "        Full Rebuild = #{rebuild}"
  puts "        Make GameLib = #{gamelib}"
  puts "         Make LogLib = #{loglib}"
  puts "      Target Country = #{target_country}"
  puts "             Version = %04d" % version
  puts "    Previous Build # = #{prev_build_num}"
  puts "     Current Build # = #{curr_build_num}"
  if anti_hack_tool == AntiHackTool::HACKSHIELD
    puts "  Hackshield Version = #{hackshield_version}"
  elsif anti_hack_tool == AntiHackTool::XIGNCODE
    puts "  XignCode Version = #{xigncode_version}"
  end
  print "Are you sure to go on with this release? [y(es)|*] : "
  answer = STDIN.gets.chomp.downcase
  unless (answer == 'y' or answer == 'yes')
    svn.revert(file_base) if buildgame or buildauth
    puts "[ERROR] Canceled by user"
    exit(-1)
  end
end

# Build binaries
begin
  if buildgame or buildauth or gamelib
    devenv.clear_build_log
  end
  if buildgame or buildauth
    # Build libraries
    devenv.solution = File.expand_path(File.join(path, solution_file))
    devenv.solution_config = 'Release - (서버용)'
    devenv.project = 'SharedLib4Build'
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
  if buildauth
    # Build auth servers
    devenv.solution = File.expand_path(File.join(path, 'Server/AuthSystem/AuthSystem.sln'))
    devenv.solution_config = 'Release'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
  if buildgame
    # Build account DB
    devenv.solution = File.expand_path(File.join(path, solution_file))
    devenv.solution_config = 'Release - (AccountDB)'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
    # Build game servers
    devenv.solution = File.expand_path(File.join(path, solution_file))
    devenv.solution_config = 'Release - (서버용)'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
  if gamelib
    # Build for client library release
    devenv.solution = File.expand_path(File.join(path, solution_file))
    devenv.solution_config = 'Release'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
rescue
  svn.revert(file_base)
  puts "[ERROR] Release stopped on build error. See 'build_log.txt' for details."
  exit(devenv.last_error)
else
  if buildgame or buildauth
    # Commit build number change
    svn.commit(file_base, "[#{target_country}] Increased #{version_str} server build number : #{prev_build_num} => #{curr_build_num}")

    unless no_update
      # Tag it!
      rev_num = svn.get_rev_num(path)
      tag_url = "svn://10.101.12.240/sun/tags/release/#{tag_target}/SUNServer_#{country_abbrev}_#{full_version}_#{today}"
      if svn.copy("#{url}@#{rev_num}", tag_url, "[#{target_country}] SUNServer_#{country_abbrev}_#{full_version}_#{today} : auto-tagged by release script\nfrom #{url}@#{rev_num}") == false
        puts "[ERROR] Couldn't copy a tag of the source tree"
        # exit(svn.last_error)
      end
    end
  end
end

# DB connection info injection
if buildauth
  Dir.chdir('../AuthSystem/Sun_DBCrypt') do
    system("InjectAuthDBInfo.#{country_abbrev}.bat")
  end
end
# xDBProxy Injection은 이제 빌드 후 이벤트로 수행됨

puts "Completed building binaries"
  
# Copy binaries

if buildauth
  target_path = File.join(release_target, 'AuthServers')
  File.makedirs(target_path)

  FileUtils.cp('../AuthSystem/_bin/AuthAgent.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/AuthServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/LoginFront.exe', target_path, :verbose => !quiet)

  FileUtils.cp('../_bin/dbghelp.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SolarDatabase.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SolarLog.dll', target_path, :verbose => !quiet)
  #FileUtils.cp('../OneWayCrypt/Bin/WZMsgDigest_x64.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../OneWayCrypt/Bin/WZMsgDigest_x86.dll', target_path, :verbose => !quiet)

  # 일본 PMS 지원
  if target_country == '_JAPAN'
    FileUtils.cp('../_bin/BDBG.dll', target_path, :verbose => !quiet)
    FileUtils.cp('../_bin/BMB.dll', target_path, :verbose => !quiet)
    FileUtils.cp('../_bin/PMSConn.dll', target_path, :verbose => !quiet)
  end

  # Backup debug info
  target_path = File.join(release_target, 'AuthDebugInfo')
  File.makedirs(target_path)

  FileUtils.cp('../AuthSystem/_bin/AuthAgent.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/AuthServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/LoginFront.exe', target_path, :verbose => !quiet)

  FileUtils.cp('../AuthSystem/_bin/AuthAgent.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/AuthServer.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/LoginFront.pdb', target_path, :verbose => !quiet)

  FileUtils.cp('../AuthSystem/_bin/AuthAgent.map', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/AuthServer.map', target_path, :verbose => !quiet)
  FileUtils.cp('../AuthSystem/_bin/LoginFront.map', target_path, :verbose => !quiet)
end

if buildgame
  target_path = File.join(release_target, 'GameServers')
  File.makedirs(target_path)

  FileUtils.cp('../_bin/AccountDatabaseProxy.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/AgentServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/BattleServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/DatabaseProxy.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/FieldServer.exe', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/GuildServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/MasterServer.exe', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/PortalServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/WorldServer.exe', target_path, :verbose => !quiet)

  FileUtils.cp('../../SUN3DEngine/dll/3DTerrain_s.dll', target_path, :verbose => !quiet)

  FileUtils.cp('../_bin/dbghelp.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/msvcp71.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/msvcr71.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SolarDatabase.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SolarLog.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SunWriteLogMng.dll', target_path, :verbose => !quiet)
  #FileUtils.cp('../OneWayCrypt/Bin/WZMsgDigest_x64.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../OneWayCrypt/Bin/WZMsgDigest_x86.dll', target_path, :verbose => !quiet)

  # 일본 PMS 지원
  if target_country == '_JAPAN'
    FileUtils.cp('../_bin/BDBG.dll', target_path, :verbose => !quiet)
    FileUtils.cp('../_bin/BMB.dll', target_path, :verbose => !quiet)
    FileUtils.cp('../_bin/PMSConn.dll', target_path, :verbose => !quiet)
  end

  if anti_hack_tool == AntiHackTool::XIGNCODE
    # Copy xigncode module files
    temp_folder = "XignCode/#{xigncode_version}"
    target_path = File.join(target_path, temp_folder)
    File.makedirs(target_path)
    if xigncode_version == 0
        xigncode_folder = "../_bin/XignCode"
    elsif
      xigncode_folder = "../_bin/XignCode/#{xigncode_version}"
    end
    
    if xigncode_version <= 110331
        FileUtils.cp("#{xigncode_folder}/ncb.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zce.xem", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zcev.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zcevlibs.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_coredb.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_helper.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_linker.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_server.dll", target_path, :verbose => !quiet)
    elsif xigncode_version <= 111215
        FileUtils.cp("#{xigncode_folder}/config.xml", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/xneutron.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/xpot.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_helper.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_linker.dll", target_path, :verbose => !quiet)
    elsif xigncode_version <= 130204
        FileUtils.cp("#{xigncode_folder}/config.xml", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/xneutron.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/xpot.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_helper.dll", target_path, :verbose => !quiet)
        FileUtils.cp("#{xigncode_folder}/zwave_sdk_linker.dll", target_path, :verbose => !quiet)
    end
  elsif anti_hack_tool == AntiHackTool::HACKSHIELD
    # Copy hackshield module files of the specified hackshield version
    hackshield_folder = "../_bin/HShield/#{hackshield_version}"
    hackshield_folder.concat('D') if target_country == '_CHINA'
    FileUtils.cp("#{hackshield_folder}/3N.mhe", target_path, :verbose => !quiet)
    FileUtils.cp("#{hackshield_folder}/AntiCpXSvr.dll", target_path, :verbose => !quiet)
    FileUtils.cp("#{hackshield_folder}/HShield.dat", target_path, :verbose => !quiet)
    FileUtils.cp("#{hackshield_folder}/HSPub.key", target_path, :verbose => !quiet)
  end
  
  Dir.chdir(release_target) do
    cmd = "7z a SUNServer_#{country_abbrev}_#{full_version}_#{today}.zip GameServers"
    cmd = cmd.concat(' >> nul') if quiet
    system(cmd)
  end

  # Backup debug info
  target_path = File.join(release_target, 'DebugInfo')
  File.makedirs(target_path)

  FileUtils.cp('../_bin/AccountDatabaseProxy.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/AgentServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/BattleServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/DatabaseProxy.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/FieldServer.exe', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/GuildServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/MasterServer.exe', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/PortalServer.exe', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/WorldServer.exe', target_path, :verbose => !quiet)

  FileUtils.cp('../_bin/AccountDatabaseProxy.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/AgentServer.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/DatabaseProxy.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/GameServer.pdb', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/GuildServer.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/MasterServer.pdb', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/PortalServer.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SunWriteLogMng.pdb', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/WorldServer.pdb', target_path, :verbose => !quiet)

  FileUtils.cp('../_bin/AccountDatabaseProxy.map', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/AgentServer.map', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/DatabaseProxy.map', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/GameServer.map', target_path, :verbose => !quiet)
  #FileUtils.cp('../_bin/GuildServer.map', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/MasterServer.map', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/WorldServer.map', target_path, :verbose => !quiet)
end

# Release game logic library
if gamelib
  # Build game logic component
  devenv.solution = File.expand_path(File.join(path, 'GameLogic/gameLogic_Sun.sln'))
  devenv.solution_config = "Release_#{logic_target}"
  devenv.project = ''
  if devenv.build(rebuild) == false
    puts "[ERROR] Error building game logic component"
    exit(devenv.last_error)
  end

  # Copy it!
  source_path = "../../GameLogic/_bin/#{country_abbrev}/gameLogic_Sun.dll"
  target_path = File.join(release_target, "SUN로직_#{country_abbrev}_#{full_version}_#{today}")
  FileUtils.remove_dir(target_path, true)
  File.makedirs(target_path)
  FileUtils.cp(source_path, target_path, :verbose => !quiet)

  Dir.chdir(target_path) do
    cmd = "7z a ../SUN로직_#{country_abbrev}_#{full_version}_#{today}.zip"
    cmd = cmd.concat(' >> nul') if quiet
    system(cmd)
  end
end

# Release game log library
if loglib
  # Build game log collector
  devenv.solution = File.expand_path(File.join(path, 'LogCollector/SUNLogConsole/SUNLogConsole.sln'))
  devenv.solution_config = "Release_#{country_abbrev}"
  devenv.project = ''
  if devenv.build(rebuild) == false
    puts "[ERROR] Error building game log collector"
    exit(devenv.last_error)
  end

  # Copy them!

  source_path = '../SunWriteLog/LogCategoryCode.h'
  target_path = File.join(release_target, "SUN로그_#{country_abbrev}_#{full_version}_#{today}")
  FileUtils.remove_dir(target_path, true)
  File.makedirs(target_path)
  FileUtils.cp(source_path, target_path, :verbose => !quiet)

  source_path = "../../LogCollector/_bin/#{country_abbrev}/SUNLogConsole.exe"
  target_path = File.join(release_target, "SUN_LogCollector_#{country_abbrev}_#{full_version}_#{today}")
  FileUtils.remove_dir(target_path, true)
  File.makedirs(target_path)
  FileUtils.cp(source_path, target_path, :verbose => !quiet)
  source_path = "../../LogCollector/_bin/#{country_abbrev}/SUNRealTimeLogConsole.exe"
  FileUtils.cp(source_path, target_path, :verbose => !quiet)
  # Copy DLLs
  FileUtils.cp('../_bin/SolarLog.dll', target_path, :verbose => !quiet)
  FileUtils.cp('../_bin/SunWriteLogMng.dll', target_path, :verbose => !quiet)

  Dir.chdir(target_path) do
    cmd = "7z a ../SUN_LogCollector_#{country_abbrev}_#{full_version}_#{today}.zip"
    cmd = cmd.concat(' >> nul') if quiet
    system(cmd)
  end
end

# Tag source on build without update
if no_update
  tag_url = "svn://10.101.12.240/SUN/tags/release/#{tag_target}/SUNServer_#{country_abbrev}_#{full_version}_#{today}"
  svn.copy('../..', tag_url, "[#{target_country}] SUNServer_#{country_abbrev}_#{full_version}_#{today} : auto-tagged by release script\nfrom working copy")
end

puts "[OK] Release completed" unless quiet

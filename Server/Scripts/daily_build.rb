require 'fileutils'
require 'ftools'
require 'getoptlong'

require 'msdev.rb'
require 'svnclient.rb'
require 'perforce.rb'
require 'simple_smtp.rb'

# global variables for command-line arguments
username = '' # subversion username
password = '' # subversion password
buildgame = false # whether to build game server group also (true/false)
buildauth = false # whether to build auth server group also (true/false)
rebuild = false # whether to perform full rebuild (true/false)
gamelib = false # whether to include game logic library release (true/false) 
p4root = 'D:\Perforce' # perforce clientspec root local directory
target_root = 'D:\Release.DailyBuilds' # release target root directory

url = '' # reposotiry source URL

path = '../..'
target_country = ''

# Multiple anti-hack tool support
class AntiHackTool
  NONE = 0
  HACKSHIELD = 1
  FROST = 2 # Russia
end
anti_hack_tool = AntiHackTool::NONE

opts = GetoptLong.new(
  [ '--username', '-u', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--password', '-p', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--buildgame', '-b', GetoptLong::NO_ARGUMENT ],
  [ '--buildauth', '-a', GetoptLong::NO_ARGUMENT ],
  [ '--rebuild', '-r', GetoptLong::NO_ARGUMENT ],
  [ '--gamelib', '-g', GetoptLong::NO_ARGUMENT ],
  [ '--p4root', '-c', GetoptLong::REQUIRED_ARGUMENT ],
  [ '--target', '-t', GetoptLong::REQUIRED_ARGUMENT ]
)

opts.each do |opt, arg|
  case opt
    when '--username' then username = arg
    when '--password' then password = arg
    when '--buildgame' then buildgame = true
    when '--buildauth' then buildauth = true
    when '--rebuild' then rebuild = true
    when '--gamelib' then gamelib = true
    when '--p4root' then p4root = arg
    when '--target' then target_root = arg
  end
end

$result_dir = 'daily_build_result'
File.makedirs($result_dir)

$today = Date.today.strftime("%y%m%d") # date string of format "yymmdd"

def write_result(result, message, revision)
  subject = "#{$result_dir}/#{$today}_#{result ? 'OK' : 'ERROR'}#{revision ? '@' + revision : ''}"
  f = File.new("#{subject}.txt", 'w')
  f.write(message)
  f.close
  
  unless result
    # e-mail on error
    smtp = SimpleSmtp.new
    smtp.smtp_host = 'mail.webzendev.com'
    smtp.to = 'jaykang@webzendev.com'
    smtp.to_alias = 'Daily Build Manager'
    smtp.from = 'no-reply@webzendev.com'
    smtp.from_alias = 'Daily Build System'
    smtp.subject = subject
    smtp.message = message
    smtp.send
  end
end

if not (buildgame or buildauth or gamelib)
  write_result(true, "Nothing to do", nil)
  exit(1)
end

if ARGV.length < 1
  write_result(false, "No repository URL specified", nil)
  exit(-1)
end

url = ARGV[0]

svn = SvnClient.new
svn.username = username
svn.password = password
svn.quiet = false

devenv = DevEnv.new
devenv.quiet = false
devenv.log_file = "#{$result_dir}\\daily_build_log.txt"

p4 = P4Client.new
p4.bin_path = '..\_bin\util'
p4.quiet = false

# Update working copy
if svn.switch(url, path) == false
  write_result(false, "Couldn't switch to \"#{url}\" on \"#{File.expand_path(path)}\"", nil)
  exit(svn.last_error)
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
country_abbrev = ''
case target_country
  when '_KOREA'
    country_abbrev = 'kr'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_JAPAN'
    country_abbrev = 'jp'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_USA'
    country_abbrev = 'us'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_TAIWAN'
    country_abbrev = 'tw'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_CHINA'
    country_abbrev = 'cn'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_GLOBAL'
    country_abbrev = 'gp'
    anti_hack_tool = AntiHackTool::HACKSHIELD
  when '_RUSSIA'
    country_abbrev = 'ru'
    anti_hack_tool = AntiHackTool::FROST
end

rev_num = svn.get_rev_num(path)

release_target = File.join(target_root, country_abbrev)
release_target = File.join(release_target, "SUNServer_#{country_abbrev}_r#{rev_num}_#{$today}")

# Make release target directory: create _n++ if it already exists
actual_release_target = release_target
next_seq = 1
while File.exist?(actual_release_target)
  actual_release_target = release_target + "_#{next_seq}"
  next_seq = next_seq + 1
end
release_target = actual_release_target

# Build binaries
begin
  devenv.clear_build_log
  if buildgame or buildauth
    # Build libraries
    devenv.solution = File.expand_path(File.join(path, 'Server/SUNServer.sln'))
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
    devenv.solution = File.expand_path(File.join(path, 'Server/SUNServer.sln'))
    devenv.solution_config = 'Release - (AccountDB)'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
    # Build game servers
    devenv.solution = File.expand_path(File.join(path, 'Server/SUNServer.sln'))
    devenv.solution_config = 'Release - (서버용)'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
  if gamelib
    # Build for client library release
    devenv.solution = File.expand_path(File.join(path, 'Server/SUNServer.sln'))
    devenv.solution_config = 'Release'
    devenv.project = ''
    if devenv.build(rebuild) == false
      raise "Build error #{devenv.last_error}"
    end
  end
rescue
  write_result(false, "Release stopped on build error. See 'daily_build_log.txt' for details.", rev_num)
  exit(devenv.last_error)
end

# DB connection info injection
if buildauth
  Dir.chdir('../AuthSystem/Sun_DBCrypt') do
    system("InjectAuthDBInfo.kr.bat")
  end
end
# xDBProxy Injection은 이제 빌드 후 이벤트로 수행됨

write_result(true, "Daily build completed", rev_num)

if buildgame
  File.makedirs(release_target)
  
  # Copy binaries
  target_path = File.join(release_target, 'GameServers')
  File.makedirs(target_path)

  FileUtils.cp('../_bin/AccountDatabaseProxy.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/AgentServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/BattleServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/DatabaseProxy.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/FieldServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/GuildServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/MasterServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/PortalServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/WorldServer.exe', target_path, :verbose => true)

  FileUtils.cp('../_bin/3DTerrain_s.dll', target_path, :verbose => true)

  FileUtils.cp('../_bin/dbghelp.dll', target_path, :verbose => true)
  FileUtils.cp('../_bin/msvcp71.dll', target_path, :verbose => true)
  FileUtils.cp('../_bin/msvcr71.dll', target_path, :verbose => true)
  FileUtils.cp('../_bin/SolarDatabase.dll', target_path, :verbose => true)
  FileUtils.cp('../_bin/SolarLog.dll', target_path, :verbose => true)
  FileUtils.cp('../_bin/SunWriteLogMng.dll', target_path, :verbose => true)

  if anti_hack_tool == AntiHackTool::HACKSHIELD
    # Copy hackshield module files of the specified hackshield version
    FileUtils.cp("../_bin/3N.mhe", target_path, :verbose => true)
    FileUtils.cp("../_bin//AntiCpXSvr.dll", target_path, :verbose => true)
    FileUtils.cp("../_bin//HShield.dat", target_path, :verbose => true)
    FileUtils.cp("../_bin//HSPub.key", target_path, :verbose => true)
  end

  # Backup debug info
  target_path = File.join(release_target, 'DebugInfo')
  File.makedirs(target_path)

  FileUtils.cp('../_bin/AccountDatabaseProxy.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/AgentServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/BattleServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/DatabaseProxy.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/FieldServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/GuildServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/MasterServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/PortalServer.exe', target_path, :verbose => true)
  FileUtils.cp('../_bin/WorldServer.exe', target_path, :verbose => true)

  FileUtils.cp('../_bin/AccountDatabaseProxy.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/AgentServer.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/DatabaseProxy.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/GameServer.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/GuildServer.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/MasterServer.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/PortalServer.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/SunWriteLogMng.pdb', target_path, :verbose => true)
  FileUtils.cp('../_bin/WorldServer.pdb', target_path, :verbose => true)

  FileUtils.cp('../_bin/AccountDatabaseProxy.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/AgentServer.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/DatabaseProxy.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/GameServer.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/GuildServer.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/MasterServer.map', target_path, :verbose => true)
  FileUtils.cp('../_bin/WorldServer.map', target_path, :verbose => true)

  # Update server resource
  p4.force_sync('//test_Dev_release/2.최신개발버전/SUN_Server/...')
  
  # Push them into development server
  
  FileUtils.cp_r(release_target, "N:\\Binaries", :verbose => true)
  FileUtils.remove_dir("N:\\Binaries.Latest", true)
  File.makedirs("N:\\Binaries.Latest")
  FileUtils.cp_r(File.join(release_target, 'GameServers'), "N:\\Binaries.Latest", :verbose => true)
  FileUtils.cp_r(File.join(release_target, 'DebugInfo'), "N:\\Binaries.Latest", :verbose => true)

  target_path = File.basename(release_target)
  target_path = File.join("N:\\Resources", target_path)
  File.makedirs(target_path)
  FileUtils.cp_r("#{p4root}\\test_Dev_release\\2.최신개발버전\\SUN_Server", target_path, :verbose => true)
  FileUtils.mv(File.join(target_path, "SUN_Server"), File.join(target_path, "Data"), :verbose => true)
  FileUtils.remove_dir("N:\\Resources.Latest", true)
  File.makedirs("N:\\Resources.Latest")
  FileUtils.cp_r(File.join(target_path, "Data"), "N:\\Resources.Latest", :verbose => true)
  # We do not preserve resource file history, since it takes a lot of disk space
  FileUtils.remove_dir(target_path, true)
end

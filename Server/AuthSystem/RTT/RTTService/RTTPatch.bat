net use "\\10.101.203.201\RTTRelease" /delete
net use "\\10.101.203.201\RTTRelease" /user:administrator "@rmfnqdnpdj!1"

net stop RTTService
net stop RTTAgent

mkdir C:\RTTService
mkdir C:\RTTAgent

xcopy "\\10.101.203.201\RTTRelease\RTTService" "C:\RTTService" /y /z
xcopy "\\10.101.203.201\RTTRelease\RTTAgent" "C:\RTTAgent" /y /z

C:

cd C:\RTTService
RTTService remove
RTTService install

cd C:\RTTAgent
RTTAgent remove
RTTAgent install

net start RTTService
net start RTTAgent

net use "\\10.101.203.201\RTTRelease" /delete

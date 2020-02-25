s="[Desktop Entry]\nName=海天鹰尺子\nComment=屏幕测量工具\nExec=`pwd`/HTYRuler\nIcon=`pwd`/HTYRuler.png\nPath=`pwd`\nTerminal=false\nCategories=System;"
echo -e $s > HTYRuler.desktop
cp `pwd`/HTYRuler.desktop ~/.local/share/applications/HTYRuler.desktop
#! /bin/bash

# 这是单行注释
:<<EOF
注释1
注释2
注释3
EOF

:<<!
这是多行注释
这是多行注释
!

name=xsb

unset name

echo "hello world"

# readonly name
# declare -r tel
 name = "张明"

# export name
# declare -x name

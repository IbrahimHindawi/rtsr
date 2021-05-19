import os

const
  progName = "nprog.exe"
  parmCount = paramCount()
  parmStr = paramStr(1)

#echo parmCount
#echo parmStr

for n in 0 ..< parmCount:
  echo paramStr(n)

if parmCount < 2:
  echo "choose task! build or run"
  
elif parmCount < 3 and parmStr == "-d:build":
  exec "nim c -r --out: bin/" & progName & " src/main.nim"

elif parmCount >= 3:
  echo "too many arguments!"

#task build, "builds an example":
#  setCommand "c"
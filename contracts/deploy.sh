scp contracts/basic.cpp  eos1:/root/projects/basic

ssh -T eos1 <<EOF
  cd projects/basic
  deploy.sh basic.app basic
EOF
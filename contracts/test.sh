cleos push action basic.app addproduct '["user1","Blockchain t Shirt","large","20.00 SYS"]' -p user1
cleos push action basic.app addproduct '["user1","Blockchain Hat","Black hat with logo","11.99 SYS"]' -p user1

cleos push action basic.app modproduct '[0,"Blockchain T Shirt","Size large blockchain shirt","19.99 SYS"]' -p user1

cleos push action basic.app delproduct '[0]' -p user1
cleos push action basic.app delproduct '[1]' -p user1
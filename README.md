show databases...
#0: crashcourse
#1: information_schema
#2: mysql
#3: performance_schema
#4: sakila
#5: sys
#6: world

show tables...
#0: customers
#1: orderitems
#2: orders
#3: productnotes
#4: products
#5: vendors

#4-2-select prod_name from products
.5 ton anvil
1 ton anvil
2 ton anvil
Detonator
Bird seed
Carrots
Fuses
JetPack 1000
JetPack 2000
Oil can
Safe
Sling
TNT (1 stick)
TNT (5 sticks)

#4-3-select prod_id, prod_name, prod_price from products
ANV01   .5 ton anvil    5.99
ANV02   1 ton anvil     9.99
ANV03   2 ton anvil     14.99
DTNTR   Detonator       13
FB      Bird seed       10
FC      Carrots 2.5
FU1     Fuses   3.42
JP1000  JetPack 1000    35
JP2000  JetPack 2000    55
OL1     Oil can 8.99
SAFE    Safe    50
SLING   Sling   4.49
TNT1    TNT (1 stick)   2.5
TNT2    TNT (5 sticks)  10

#4-4-select * from products
ANV01   1001    .5 ton anvil    5.99    .5 ton anvil, black, complete with handy hook
ANV02   1001    1 ton anvil     9.99    1 ton anvil, black, complete with handy hook and carrying case
ANV03   1001    2 ton anvil     14.99   2 ton anvil, black, complete with handy hook and carrying case
DTNTR   1003    Detonator       13      Detonator (plunger powered), fuses not included
FB      1003    Bird seed       10      Large bag (suitable for road runners)
FC      1003    Carrots 2.5     Carrots (rabbit hunting season only)
FU1     1002    Fuses   3.42    1 dozen, extra long
JP1000  1005    JetPack 1000    35      JetPack 1000, intended for single use
JP2000  1005    JetPack 2000    55      JetPack 2000, multi-use
OL1     1002    Oil can 8.99    Oil can, red
SAFE    1003    Safe    50      Safe with combination lock
SLING   1003    Sling   4.49    Sling, one size fits all
TNT1    1003    TNT (1 stick)   2.5     TNT, red, single stick
TNT2    1003    TNT (5 sticks)  10      TNT, red, pack of 10 sticks

#4-5-select distinct vend_id from products
1001
1002
1003
1005

#4-5-select vend_id from products group by vend_id
1001
1002
1003
1005

#4-6-select prod_name from products limit 5
.5 ton anvil
1 ton anvil
2 ton anvil
Detonator
Bird seed

#4-6-select prod_name from products limit 3,4
Detonator
Bird seed
Carrots
Fuses

#5-3-select prod_id, prod_price, prod_name from products order by prod_price, prod_name
FC      2.5     Carrots
TNT1    2.5     TNT (1 stick)
FU1     3.42    Fuses
SLING   4.49    Sling
ANV01   5.99    .5 ton anvil
OL1     8.99    Oil can
ANV02   9.99    1 ton anvil
FB      10      Bird seed
TNT2    10      TNT (5 sticks)
DTNTR   13      Detonator
ANV03   14.99   2 ton anvil
JP1000  35      JetPack 1000
SAFE    50      Safe
JP2000  55      JetPack 2000

#5-3-select prod_id, prod_price, prod_name from products order by prod_price DESC
JP2000  55      JetPack 2000
SAFE    50      Safe
JP1000  35      JetPack 1000
ANV03   14.99   2 ton anvil
DTNTR   13      Detonator
FB      10      Bird seed
TNT2    10      TNT (5 sticks)
ANV02   9.99    1 ton anvil
OL1     8.99    Oil can
ANV01   5.99    .5 ton anvil
SLING   4.49    Sling
FU1     3.42    Fuses
FC      2.5     Carrots
TNT1    2.5     TNT (1 stick)

#5-3-select prod_price from products order by prod_price DESC limit 1
55

#6-1-select prod_name, prod_price from products where prod_price=2.50
Carrots
TNT (1 stick)

#6-2--select prod_name, prod_price from products where prod_name = 'fuses'
Fuses   3.42

#6-2-select prod_name, prod_price from products where prod_price<=10
.5 ton anvil    5.99
1 ton anvil     9.99
Bird seed       10
Carrots 2.5
Fuses   3.42
Oil can 8.99
Sling   4.49
TNT (1 stick)   2.5
TNT (5 sticks)  10

#6-2-select vend_id, prod_name from products where vend_id<>1003
1001    .5 ton anvil
1001    1 ton anvil
1001    2 ton anvil
1002    Fuses
1002    Oil can
1005    JetPack 1000
1005    JetPack 2000

#6-2-select prod_name, prod_price from products where prod_price between 5 and 10
.5 ton anvil    5.99
1 ton anvil     9.99
Bird seed       10
Oil can 8.99
TNT (5 sticks)  10

#4-5-select cust_id from customers where cust_email is null
10002
10005

#7-1-select prod_id, prod_price, prod_name from products where vend_id=1003 and prod_price<=10
FB      10      Bird seed
FC      2.5     Carrots
SLING   4.49    Sling
TNT1    2.5     TNT (1 stick)
TNT2    10      TNT (5 sticks)

#7-1-select prod_name, prod_price from products where vend_id=1002 or vend_id=1003
Fuses   3.42
Oil can 8.99
Detonator       13
Bird seed       10
Carrots 2.5
Safe    50
Sling   4.49
TNT (1 stick)   2.5
TNT (5 sticks)  10

#7-1-select prod_name, prod_price from products where vend_id=1002 or vend_id=1003
Fuses   3.42
Oil can 8.99
Detonator       13
Bird seed       10
Carrots 2.5
Safe    50
Sling   4.49
TNT (1 stick)   2.5
TNT (5 sticks)  10

#7-1-select prod_name, prod_price from products where (vend_id=1002 or vend_id=1003) and prod_price>=10
Detonator       13
Bird seed       10
Safe    50
TNT (5 sticks)  10

#7-2-select prod_name, prod_price from products where vend_id in (1002,1003) order by prod_name
Bird seed       10
Carrots 2.5
Detonator       13
Fuses   3.42
Oil can 8.99
Safe    50
Sling   4.49
TNT (1 stick)   2.5
TNT (5 sticks)  10

#7-3-select prod_name, prod_price from products where vend_id not in (1002,1003) order by prod_name
.5 ton anvil    5.99
1 ton anvil     9.99
2 ton anvil     14.99
JetPack 1000    35
JetPack 2000    55

#8-1-select prod_id, prod_name from products where prod_name like 'jet%'
JP1000  JetPack 1000
JP2000  JetPack 2000

#8-1-select prod_id, prod_name from products where prod_name like '%anvil%'
ANV01   .5 ton anvil
ANV02   1 ton anvil
ANV03   2 ton anvil

#8-1-select prod_id, prod_name from products where prod_name like '_ ton anvil'
ANV02   1 ton anvil
ANV03   2 ton anvil

#9-1-select prod_name from products where prod_name regexp '1000' order by prod_name
JP1000  JetPack 1000

#9-1-select prod_name from products where prod_name regexp '.000' order by prod_name
JP1000  JetPack 1000
JP2000  JetPack 2000

#9-2-select prod_name from products where prod_name regexp '1000|2000' order by prod_name
JP1000  JetPack 1000
JP2000  JetPack 2000

#9-2-select prod_name from products where prod_name regexp '[123] Ton' order by prod_name
ANV02   1 ton anvil
ANV03   2 ton anvil

#9-2-select prod_name from products where prod_name regexp '1|2|3 Ton' order by prod_name
ANV02   1 ton anvil
ANV03   2 ton anvil
JP1000  JetPack 1000
JP2000  JetPack 2000
TNT1    TNT (1 stick)

#9-2-select prod_name from products where prod_name regexp '[1-5] Ton' order by prod_name
ANV01   .5 ton anvil
ANV02   1 ton anvil
ANV03   2 ton anvil

#9-2-select vend_name from vendors where vend_name regexp '.' order by vend_name
ACME
Anvils R Us
Furball Inc.
Jet Set
Jouets Et Ours
LT Supplies

#9-2-select vend_name from vendors where vend_name regexp '\.' order by vend_name
Furball Inc.

#9-2-select prod_name from products where prod_name regexp '\([0-9] sticks?\)' order by prod_name
TNT (1 stick)
TNT (5 sticks)

#9-2-select prod_name from products where prod_name regexp '[[:digit:]]{4}' order by prod_name
JetPack 1000
JetPack 2000

#9-2-select prod_name from products where prod_name regexp '^[0-9\.]' order by prod_name
.5 ton anvil
1 ton anvil
2 ton anvil

#10-2-select concat(vend_name,'(', vend_country, ')') from vendors order by vend_name
ACME(USA)
Anvils R Us(USA)
Furball Inc.(USA)
Jet Set(England)
Jouets Et Ours(France)
LT Supplies(USA)

#10-2-select concat(rtrim(vend_name),'(', rtrim(vend_country), ')') from vendors order by vend_name
ACME(USA)
Anvils R Us(USA)
Furball Inc.(USA)
Jet Set(England)
Jouets Et Ours(France)
LT Supplies(USA)

#10-2-select prod_id, quantity, item_price from orderitems where order_num = 20005
ANV01   10      5.99    59.9
ANV02   3       9.99    29.97
TNT2    5       10      50
FB      1       10      10

#11-2-select vend_name, upper(vend_name) as vend_name_upcase from vendors order by vend_name
ACME    ACME
Anvils R Us     ANVILS R US
Furball Inc.    FURBALL INC.
Jet Set JET SET
Jouets Et Ours  JOUETS ET OURS
LT Supplies     LT SUPPLIES

#11-2-select cust_name, cust_contact from customers where soundex(cust_contact)=soundex('Y Lie')
Coyote Inc.     Y Lee

#11-2-select cust_id, order_num from orders where order_date='2005-09-01'
10001   20005

#11-2-select cust_id, order_num from orders where date(order_date) between '2005-09-01' and '2005-09-30'
10001   20005
10003   20006
10004   20007

#11-2-select avg(prod_price) as avg_price from products where vend_id=1003
13.2129

#11-2-select count(cust_email) as num_cust from customers
3

#11-2-select max(prod_price) as max_price from products
55

#11-2-select min(prod_price) as min_price from products
2.5

#11-2-select sum(quantity) as items_ordered from orderitems where order_num =20005
19

#11-2-select sum(item_price*quantity) as total_price from orderitems where order_num =20005
149.87

#11-2-select avg(distinct prod_price) as avg_price from products where vend_id=1003
15.998

#11-2-select count(*) as num_prods from products where vend_id=1003
7

#13-2-select vend_id, count(*) as num_prods from products groud by vend_id
1001    3
1002    2
1003    7
1005    2

#13-2-select cust_id, count(*) as orders from orders group by cust_id having count(*)>=2
10001   2

#13-3-select vend_id, count(*) as num_prods from products where prod_price>=10 group by vend_id having(count(*)>=2)
1003    4
1005    2

#13-4-select order_num, sum(quantity*item_price) as ordertotal from orderitems group by order_num having ordertotal>=50
order by ordertotal
20006   55
20008   125
20005   149.87
20007   1000

#13-4-select order_num, sum(quantity*item_price) as ordertotal from orderitems group by order_num having ordertotal>=50
order by ordertotal
20006   55
20008   125
20005   149.87
20007   1000

#14-2-select cust_id from orders where order_num in (select order_num from orderitems where prod_id='TNT2')
10001
10004

#19-2-insert into customers value(10006,'Pep E. LaPew', '100 main Sstreet', 'Los Angeles', 'CA', '90046','USA',null,null
);
10006
1
#20-1-update customers set cust_email = 'elmer@fudd.com' where cust_id =10005
0
1
#20-1-update customers set cust_email = 'elmer@fudd.com' where cust_id =10005
0
1
#20-2-delete from customers where cust_id=10006
0
1

D:\c++\mysql_test\x64\Debug\mysql_test.exe (进程 1796)已退出，返回代码为: 0。
按任意键关闭此窗口...

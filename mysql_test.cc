// mysql必知必会 结合mysql8.0 xdevapi 测试

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <mysqlx/xdevapi.h>

struct
{
  std::string host = "localhost";
  unsigned short port = 33060;

  std::string user = "root";
  std::string pwd = "353213";

  std::string db = "crashcourse";

  // tables
  std::string customers = "customers";
  std::string orderitems = "orderitems";
  std::string orders = "orders";
  std::string productnotes = "productnotes";
  std::string products = "products";
  std::string vendors = "vendors";
} config;

using std::cout;
using std::endl;
using namespace mysqlx;

std::ostream &operator<<(std::ostream &out, RowResult &res)
{
  while (Row row = res.fetchOne()) {
    for (unsigned i = 0; i < row.colCount(); ++i) {
      out << row.get(i) << '\t';
    }
    out << endl;
  }
  return out;
}

int main(int argc)
{
  try {
    Session sess("localhost", 33060, "root", "353213");
    // get version
    RowResult res = sess.sql("show variables like 'version'").execute();
    std::stringstream version;

    version << res.fetchOne().get(1).get<string>();
    int major_version;
    version >> major_version;
    if (major_version < 8) {
      cout << "major_version < 8!" << endl;
      return 0;
    }

    std::vector<Schema> schemas = sess.getSchemas();
    // show databases
    cout << "show databases..." << endl;
    for (unsigned i = 0; i < schemas.size(); ++i) {
      cout << "#" << i << ": " << schemas[i].getName() << endl;
    }
    cout << endl;

    Schema sch = sess.getSchema(config.db, true);
    // show tables
    cout << "show tables..." << endl;
    std::vector<Table> tables = sch.getTables();
    for (unsigned i = 0; i < tables.size(); ++i) {
      cout << "#" << i << ": " << tables[i].getName() << endl;
    }
    cout << endl;

    Table products = sch.getTable(config.products);
    Table vendors = sch.getTable(config.vendors);
    Table customers = sch.getTable(config.customers);
    Table orderitems = sch.getTable(config.orderitems);
    Table orders = sch.getTable(config.orders);

    // select
    cout << "#001-select prod_name from products\n";
    res = products.select("prod_name").execute();
    cout << res << endl;

    // select ...
    cout << "#002-select prod_id, prod_name, prod_price from products\n";
    res = products.select("prod_id", "prod_name", "prod_price").execute();
    cout << res << endl;

    // select *
    cout << "#003-select * from products\n";
    res = products.select("*").execute();
    cout << res << endl;

    // select
    cout << "#004-select vend_id from products\n";
    res = products.select("vend_id").execute();
    cout << res << endl;

    // groupBy 去重复
    cout << "#005-select vend_id from products\n";
    res = products.select("vend_id").groupBy("vend_id").execute();
    cout << res << endl;

    // limit 限制数目
    cout << "#006-select prod_name from products limit 5\n";
    res = products.select("prod_name").limit(5).execute();
    cout << res << endl;

    // offset 偏移数
    cout << "#007-select prod_name from products limit 3,4\n";
    res = products.select("prod_name").limit(4).offset(3).execute();  // 第3行开始选4列
    cout << res << endl;

    // orderBy 排序升序ASC
    cout << "#008-select prod_id, prod_price, prod_name from products order by prod_price, prod_name\n";
    res = products.select("prod_id", "prod_price", "prod_name").orderBy("prod_price", "prod_name").execute();
    cout << res << endl;

    // orderBy 排序降序DESC
    cout << "#009-select prod_id, prod_price, prod_name from products order by prod_price DESC\n";
    res = products.select("prod_id", "prod_price", "prod_name").orderBy("prod_price DESC").execute();
    cout << res << endl;

    // orderBy().limit()
    cout << "#010-select prod_price from products order by prod_price DESC limit 1\n";
    res = products.select("prod_price").orderBy("prod_price DESC").limit(1).execute();
    cout << res << endl;

    // =
    cout << "#011-select prod_name, prod_price from products where prod_price=2.50\n";
    res = products.select("prod_name").where("prod_price=2.50").execute();
    cout << res << endl;

    // =
    cout << "#012-select prod_name, prod_price from products where prod_name = 'fuses'\n";
    res = products.select("prod_name", "prod_price").where("prod_name='fuses'").execute();
    cout << res << endl;

    // <=
    cout << "#013-select prod_name, prod_price from products where prod_price<=10\n";
    res = products.select("prod_name", "prod_price").where("prod_price<=10").execute();
    cout << res << endl;

    // <>
    cout << "#014-select vend_id, prod_name from products where vend_id<>1003\n";
    res = products.select("vend_id", "prod_name").where("vend_id<>1003").execute();
    cout << res << endl;

    // between and
    cout << "#015-select prod_name, prod_price from products where prod_price between 5 and 10\n";
    res = products.select("prod_name", "prod_price").where("prod_price between 5 and 10").execute();
    cout << res << endl;

    // is null
    cout << "#016-select cust_id from customers where cust_email is null\n";
    res = customers.select("cust_id").where("cust_email is null").execute();
    cout << res << endl;

    // and
    cout << "#017-select prod_id, prod_price, prod_name from products where vend_id=1003 and prod_price<=10\n";
    res = products.select("prod_id", "prod_price", "prod_name").where("vend_id=1003 and prod_price<=10").execute();
    cout << res << endl;

    // or
    cout << "#018-select prod_name, prod_price from products where vend_id=1002 or vend_id=1003\n";
    res = products.select("prod_name", "prod_price").where("vend_id=1002 or vend_id=1003").execute();
    cout << res << endl;

    // or
    cout << "#019-select prod_name, prod_price from products where vend_id=1002 or vend_id=1003\n";
    res = products.select("prod_name", "prod_price").where("vend_id=1002 or vend_id=1003").execute();
    cout << res << endl;

    // or and
    cout << "#020-select prod_name, prod_price from products where (vend_id=1002 or vend_id=1003) and prod_price>=10\n";
    res =
        products.select("prod_name", "prod_price").where("(vend_id=1002 or vend_id=1003) and prod_price>=10").execute();
    cout << res << endl;

    // in
    cout << "#021-select prod_name, prod_price from products where vend_id in (1002,1003) order by prod_name\n";
    res = products.select("prod_name", "prod_price").where("vend_id in (1002,1003)").orderBy("prod_name").execute();
    cout << res << endl;

    // not in
    cout << "#022-select prod_name, prod_price from products where vend_id not in (1002,1003) order by prod_name\n";
    res = products.select("prod_name", "prod_price").where("vend_id not in(1002,1003)").orderBy("prod_name").execute();
    cout << res << endl;

    // like 'jet%'
    cout << "#023-select prod_id, prod_name from products where prod_name like 'jet%'\n";
    res = products.select("prod_id", "prod_name").where("prod_name like 'jet%'").execute();
    cout << res << endl;

    // like '%anvil%'
    cout << "#024-select prod_id, prod_name from products where prod_name like '%anvil%'\n";
    res = products.select("prod_id", "prod_name").where("prod_name like '%anvil%'").execute();
    cout << res << endl;

    // like '_ ton anvil'
    cout << "#025-select prod_id, prod_name from products where prod_name like '_ ton anvil'\n";
    res = products.select("prod_id", "prod_name").where("prod_name like '_ ton anvil'").execute();
    cout << res << endl;

    // regexp '1000'
    cout << "#026-select prod_name from products where prod_name regexp '1000' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '1000'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '.000'
    cout << "#027-select prod_name from products where prod_name regexp '.000' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '.000'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '1000|2000'
    cout << "#028-select prod_name from products where prod_name regexp '1000|2000' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '1000|2000'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '[123] Ton'
    cout << "#029-select prod_name from products where prod_name regexp '[123] Ton' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '[123] Ton'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '1|2|3 Ton'
    cout << "#030-select prod_name from products where prod_name regexp '1|2|3 Ton' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '1|2|3 Ton'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '[1-5] Ton'
    cout << "#031-select prod_name from products where prod_name regexp '[1-5] Ton' order by prod_name\n";
    res = products.select("prod_id", "prod_name").where("prod_name regexp '[1-5] Ton'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep '.'
    cout << "#032-select vend_name from vendors where vend_name regexp '.' order by vend_name\n";
    res = vendors.select("vend_name").where("vend_name regexp '.'").orderBy("vend_name").execute();
    cout << res << endl;

    // regep '\\.'
    cout << "#033-select vend_name from vendors where vend_name regexp '\\.' order by vend_name\n";
    res = vendors.select("vend_name").where("vend_name regexp '\\.'").orderBy("vend_name").execute();
    cout << res << endl;

    // regep '\\([0-9] sticks?\\)'
    cout << "#034-select prod_name from products where prod_name regexp '\\([0-9] sticks?\\)' order by prod_name\n";
    res = products.select("prod_name").where("prod_name regexp '\\([0-9] sticks?\\)'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep [[:digit:]]{4}
    cout << "#035-select prod_name from products where prod_name regexp '[[:digit:]]{4}' order by prod_name\n";
    res = products.select("prod_name").where("prod_name regexp '[[:digit:]]{4}'").orderBy("prod_name").execute();
    cout << res << endl;

    // regep ^[0-9\\.]
    cout << "#036-select prod_name from products where prod_name regexp '^[0-9\\.]' order by prod_name\n";
    res = products.select("prod_name").where("prod_name regexp '^[0-9\\.]'").orderBy("prod_name").execute();
    cout << res << endl;

    // concat
    cout << "#037-select concat(vend_name,'(', vend_country, ')') from vendors order by vend_name\n";
    res = vendors.select("concat(vend_name,'(', vend_country, ')')").orderBy("vend_name").execute();
    cout << res << endl;

    // rtrim
    cout << "#038-select concat(rtrim(vend_name),'(', rtrim(vend_country), ')') from vendors order by vend_name\n";
    res = vendors.select("concat(rtrim(vend_name),'(', rtrim(vend_country), ')')").orderBy("vend_name").execute();
    cout << res << endl;

    // as
    cout << "#039-select prod_id, quantity, item_price from orderitems where order_num = 20005\n";
    res = orderitems.select("prod_id", "quantity", "item_price", "quantity*item_price AS expanded_price")
              .where("order_num = 20005")
              .execute();
    cout << res << endl;

    // upper
    cout << "#040-select vend_name, upper(vend_name) as vend_name_upcase from vendors order by vend_name\n";
    res = vendors.select("vend_name", "upper(vend_name) as vend_name_upcase").orderBy("vend_name").execute();
    cout << res << endl;

    // soundex
    cout << "#041-select cust_name, cust_contact from customers where soundex(cust_contact)=soundex('Y Lie')\n";
    res = customers.select("cust_name", "cust_contact").where("soundex(cust_contact)=soundex('Y Lie')").execute();
    cout << res << endl;

    // date
    cout << "#042-select cust_id, order_num from orders where order_date='2005-09-01'\n";
    res = orders.select("cust_id", "order_num").where("order_date='2005-09-01'").execute();
    cout << res << endl;

    // date
    cout << "#043-select cust_id, order_num from orders where date(order_date) between '2005-09-01' and '2005-09-30'\n";
    res =
        orders.select("cust_id", "order_num").where("date(order_date) between '2005-09-01' and '2005-09-30'").execute();
    cout << res << endl;

    // avg
    cout << "#044-select avg(prod_price) as avg_price from products where vend_id=1003\n";
    res = products.select("avg(prod_price) as avg_price").where("vend_id=1003").execute();
    cout << res << endl;

    // count
    cout << "#045-select count(cust_email) as num_cust from customers\n";
    res = customers.select("count(cust_email) as num_cust").execute();
    cout << res << endl;

    // max
    cout << "#046-select max(prod_price) as max_price from products\n";
    res = products.select("max(prod_price) as max_price").execute();
    cout << res << endl;

    // min
    cout << "#046-select min(prod_price) as min_price from products\n";
    res = products.select("min(prod_price) as min_price").execute();
    cout << res << endl;

    // sum
    cout << "#047-select sum(quantity) as items_ordered from orderitems where order_num =20005\n";
    res = orderitems.select("sum(quantity) as items_ordered").where("order_num=20005").execute();
    cout << res << endl;

    // sum
    cout << "#048-select sum(item_price*quantity) as total_price from orderitems where order_num =20005\n";
    res = orderitems.select("sum(item_price*quantity) as total_price").where("order_num=20005").execute();
    cout << res << endl;

    // avg
    cout << "#049-select avg(distinct prod_price) as avg_price from products where vend_id=1003\n";
    res = products.select("avg(prod_price) as avg_price").where("vend_id=1003").execute();
    cout << res << endl;


  } catch (const mysqlx::Error &err) {
    cout << "ERROR: " << err << endl;
    return 1;
  } catch (std::exception &ex) {
    cout << "STD EXCEPTION: " << ex.what() << endl;
    return 1;
  } catch (const char *ex) {
    cout << "EXCEPTION: " << ex << endl;
    return 1;
  } catch (...) {
    cout << "UNKONW ERROR!" << endl;
    return 2;
  }

  return 0;
}
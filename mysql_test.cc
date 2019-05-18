// mysql必知必会 结合mysql8.0 xdevapi 测试 select insert update remove 

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

std::ostream &operator<<(std::ostream &out, RowResult &rowresult)
{
  while (Row row = rowresult.fetchOne()) {
    for (unsigned i = 0; i < row.colCount(); ++i) {
      out << row.get(i) << '\t';
    }
    out << endl;
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, Result &result)
{
  std::vector<string> ids = result.getGeneratedIds();
  if (!ids.empty()) {
    for (auto id : ids) {
      out << id << '\t';
    }
    out << endl;
  }
  out << result.getAutoIncrementValue() << endl;

  std::vector<Warning> ws = result.getWarnings();
  if (!ws.empty()) {
    for (auto w : ws) {
      out << w << '\t';
    }
    out << endl;
  }
  out << result.getAffectedItemsCount();

  return out;
}

int main(int argc)
{
  try {
    // get version
    Session sess("localhost", 33060, "root", "353213");
    RowResult rowresult = sess.sql("show variables like 'version'").execute();
    std::stringstream version;

    version << rowresult.fetchOne().get(1).get<string>();
    int major_version;
    version >> major_version;
    if (major_version < 8) {
      cout << "major_version < 8!" << endl;
      return 0;
    }

    // show databases
    std::vector<Schema> schemas = sess.getSchemas();
    cout << "show databases..." << endl;
    for (unsigned i = 0; i < schemas.size(); ++i) {
      cout << "#" << i << ": " << schemas[i].getName() << endl;
    }
    cout << endl;

    string sql = "use " + config.db;
    sess.sql(sql).execute();

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

    // select 检索单个列
    sql = "select prod_name from products";
    rowresult = products.select("prod_name").execute();
    cout << "#4-2-" << sql << endl << rowresult << endl;

    // select ... 检索多个列
    sql = "select prod_id, prod_name, prod_price from products";
    rowresult = products.select("prod_id", "prod_name", "prod_price").execute();
    cout << "#4-3-" << sql << endl << rowresult << endl;

    // select * 检索所有列
    sql = "select * from products";
    rowresult = products.select("*").execute();
    cout << "#4-4-" << sql << endl << rowresult << endl;

    // select 检索不同的行
    sql = "select distinct vend_id from products";
    rowresult = sess.sql(sql).execute();
    cout << "#4-5-" << sql << endl << rowresult << endl;

    // groupBy 检索不同的行
    sql = "select vend_id from products group by vend_id";
    rowresult = products.select("vend_id").groupBy("vend_id").execute();
    cout << "#4-5-" << sql << endl << rowresult << endl;

    // limit 限制结果
    sql = "select prod_name from products limit 5";
    rowresult = products.select("prod_name").limit(5).execute();
    cout << "#4-6-" << sql << endl << rowresult << endl;

    // offset 偏移数
    sql = "select prod_name from products limit 3,4";
    rowresult = products.select("prod_name").limit(4).offset(3).execute();  // 第3行开始选4列
    cout << "#4-6-" << sql << endl << rowresult << endl;

    // orderBy 排序数据 ASC升序
    sql = "select prod_id, prod_price, prod_name from products order by prod_price, prod_name";
    rowresult = products.select("prod_id", "prod_price", "prod_name").orderBy("prod_price", "prod_name").execute();
    cout << "#5-3-" << sql << endl << rowresult << endl;

    // orderBy 排序数据 DESC降序
    sql = "select prod_id, prod_price, prod_name from products order by prod_price DESC";
    rowresult = products.select("prod_id", "prod_price", "prod_name").orderBy("prod_price DESC").execute();
    cout << "#5-3-" << sql << endl << rowresult << endl;

    // order by 结合limit
    sql = "select prod_price from products order by prod_price DESC limit 1";
    rowresult = products.select("prod_price").orderBy("prod_price DESC").limit(1).execute();
    cout << "#5-3-" << sql << endl << rowresult << endl;

    // where
    sql = "select prod_name, prod_price from products where prod_price=2.50";
    rowresult = products.select("prod_name").where("prod_price=2.50").execute();
    cout << "#6-1-" << sql << endl << rowresult << endl;

    // where = 检查单个值
    sql = "-select prod_name, prod_price from products where prod_name = 'fuses'";
    rowresult = products.select("prod_name", "prod_price").where("prod_name='fuses'").execute();
    cout << "#6-2-" << sql << endl << rowresult << endl;

    // where <=
    sql = "select prod_name, prod_price from products where prod_price<=10";
    rowresult = products.select("prod_name", "prod_price").where("prod_price<=10").execute();
    cout << "#6-2-" << sql << endl << rowresult << endl;

    // where <> 不匹配检查
    sql = "select vend_id, prod_name from products where vend_id<>1003";
    rowresult = products.select("vend_id", "prod_name").where("vend_id<>1003").execute();
    cout << "#6-2-" << sql << endl << rowresult << endl;

    // between and 范围值检查
    sql = "select prod_name, prod_price from products where prod_price between 5 and 10";
    rowresult = products.select("prod_name", "prod_price").where("prod_price between 5 and 10").execute();
    cout << "#6-2-" << sql << endl << rowresult << endl;

    // where is null 空值检查
    sql = "select cust_id from customers where cust_email is null";
    rowresult = customers.select("cust_id").where("cust_email is null").execute();
    cout << "#4-5-" << sql << endl << rowresult << endl;

    // and 操作符
    sql = "select prod_id, prod_price, prod_name from products where vend_id=1003 and prod_price<=10";
    rowresult =
        products.select("prod_id", "prod_price", "prod_name").where("vend_id=1003 and prod_price<=10").execute();
    cout << "#7-1-" << sql << endl << rowresult << endl;

    // or 操作符
    sql = "select prod_name, prod_price from products where vend_id=1002 or vend_id=1003";
    rowresult = products.select("prod_name", "prod_price").where("vend_id=1002 or vend_id=1003").execute();
    cout << "#7-1-" << sql << endl << rowresult << endl;

    // or 操作符
    sql = "select prod_name, prod_price from products where vend_id=1002 or vend_id=1003";
    rowresult = products.select("prod_name", "prod_price").where("vend_id=1002 or vend_id=1003").execute();
    cout << "#7-1-" << sql << endl << rowresult << endl;

    // or < and 计算次序
    sql = "select prod_name, prod_price from products where (vend_id=1002 or vend_id=1003) and prod_price>=10";
    rowresult =
        products.select("prod_name", "prod_price").where("(vend_id=1002 or vend_id=1003) and prod_price>=10").execute();
    cout << "#7-1-" << sql << endl << rowresult << endl;

    // in 操作符
    sql = "select prod_name, prod_price from products where vend_id in (1002,1003) order by prod_name";
    rowresult =
        products.select("prod_name", "prod_price").where("vend_id in (1002,1003)").orderBy("prod_name").execute();
    cout << "#7-2-" << sql << endl << rowresult << endl;

    // not in 操作符
    sql = "select prod_name, prod_price from products where vend_id not in (1002,1003) order by prod_name";
    rowresult =
        products.select("prod_name", "prod_price").where("vend_id not in(1002,1003)").orderBy("prod_name").execute();
    cout << "#7-3-" << sql << endl << rowresult << endl;

    // like 百分号（%）通配符
    sql = "select prod_id, prod_name from products where prod_name like 'jet%'";
    rowresult = products.select("prod_id", "prod_name").where("prod_name like 'jet%'").execute();
    cout << "#8-1-" << sql << endl << rowresult << endl;

    // like 百分号（%）通配符
    sql = "select prod_id, prod_name from products where prod_name like '%anvil%'";
    rowresult = products.select("prod_id", "prod_name").where("prod_name like '%anvil%'").execute();
    cout << "#8-1-" << sql << endl << rowresult << endl;

    // like 下划线（_）通配符
    sql = "select prod_id, prod_name from products where prod_name like '_ ton anvil'";
    rowresult = products.select("prod_id", "prod_name").where("prod_name like '_ ton anvil'").execute();
    cout << "#8-1-" << sql << endl << rowresult << endl;

    // regexp 基本字符匹配
    sql = "select prod_name from products where prod_name regexp '1000' order by prod_name";
    rowresult = products.select("prod_id", "prod_name").where("prod_name regexp '1000'").orderBy("prod_name").execute();
    cout << "#9-1-" << sql << endl << rowresult << endl;

    // regep (.)任意一个字符
    sql = "select prod_name from products where prod_name regexp '.000' order by prod_name";
    rowresult = products.select("prod_id", "prod_name").where("prod_name regexp '.000'").orderBy("prod_name").execute();
    cout << "#9-1-" << sql << endl << rowresult << endl;

    // regep 进行 or 匹配
    sql = "select prod_name from products where prod_name regexp '1000|2000' order by prod_name";
    rowresult =
        products.select("prod_id", "prod_name").where("prod_name regexp '1000|2000'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep [abc] 匹配几个字符之一
    sql = "select prod_name from products where prod_name regexp '[123] Ton' order by prod_name";
    rowresult =
        products.select("prod_id", "prod_name").where("prod_name regexp '[123] Ton'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep [a|b|c] 匹配几个字符之一
    sql = "select prod_name from products where prod_name regexp '1|2|3 Ton' order by prod_name";
    rowresult =
        products.select("prod_id", "prod_name").where("prod_name regexp '1|2|3 Ton'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep [a-b] 匹配范围
    sql = "select prod_name from products where prod_name regexp '[1-5] Ton' order by prod_name";
    rowresult =
        products.select("prod_id", "prod_name").where("prod_name regexp '[1-5] Ton'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep '.'
    sql = "select vend_name from vendors where vend_name regexp '.' order by vend_name";
    rowresult = vendors.select("vend_name").where("vend_name regexp '.'").orderBy("vend_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep 匹配特殊字符 '\\.'
    sql = "select vend_name from vendors where vend_name regexp '\\.' order by vend_name";
    rowresult = vendors.select("vend_name").where("vend_name regexp '\\.'").orderBy("vend_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep 匹配特殊字符 '\\([0-9] sticks?\\)'
    sql = "select prod_name from products where prod_name regexp '\\([0-9] sticks?\\)' order by prod_name";
    rowresult =
        products.select("prod_name").where("prod_name regexp '\\([0-9] sticks?\\)'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep 匹配字符类 [[:digit:]]{4}
    sql = "select prod_name from products where prod_name regexp '[[:digit:]]{4}' order by prod_name";
    rowresult = products.select("prod_name").where("prod_name regexp '[[:digit:]]{4}'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // regep ^[0-9\\.]
    sql = "select prod_name from products where prod_name regexp '^[0-9\\.]' order by prod_name";
    rowresult = products.select("prod_name").where("prod_name regexp '^[0-9\\.]'").orderBy("prod_name").execute();
    cout << "#9-2-" << sql << endl << rowresult << endl;

    // concat 拼接字段
    sql = "select concat(vend_name,'(', vend_country, ')') from vendors order by vend_name";
    rowresult = vendors.select("concat(vend_name,'(', vend_country, ')')").orderBy("vend_name").execute();
    cout << "#10-2-" << sql << endl << rowresult << endl;

    // rtrim 去空格
    sql = "select concat(rtrim(vend_name),'(', rtrim(vend_country), ')') from vendors order by vend_name";
    rowresult = vendors.select("concat(rtrim(vend_name),'(', rtrim(vend_country), ')')").orderBy("vend_name").execute();
    cout << "#10-2-" << sql << endl << rowresult << endl;

    // as 别名
    sql = "select prod_id, quantity, item_price from orderitems where order_num = 20005";
    rowresult = orderitems.select("prod_id", "quantity", "item_price", "quantity*item_price AS expanded_price")
                    .where("order_num = 20005")
                    .execute();
    cout << "#10-2-" << sql << endl << rowresult << endl;

    // upper 大写
    sql = "select vend_name, upper(vend_name) as vend_name_upcase from vendors order by vend_name";
    rowresult = vendors.select("vend_name", "upper(vend_name) as vend_name_upcase").orderBy("vend_name").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // soundex 发音相似
    sql = "select cust_name, cust_contact from customers where soundex(cust_contact)=soundex('Y Lie')";
    rowresult = customers.select("cust_name", "cust_contact").where("soundex(cust_contact)=soundex('Y Lie')").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // date
    sql = "select cust_id, order_num from orders where order_date='2005-09-01'";
    rowresult = orders.select("cust_id", "order_num").where("order_date='2005-09-01'").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // date
    sql = "select cust_id, order_num from orders where date(order_date) between '2005-09-01' and '2005-09-30'";
    rowresult =
        orders.select("cust_id", "order_num").where("date(order_date) between '2005-09-01' and '2005-09-30'").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // avg
    sql = "select avg(prod_price) as avg_price from products where vend_id=1003";
    rowresult = products.select("avg(prod_price) as avg_price").where("vend_id=1003").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // count
    sql = "select count(cust_email) as num_cust from customers";
    rowresult = customers.select("count(cust_email) as num_cust").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // max
    sql = "select max(prod_price) as max_price from products";
    rowresult = products.select("max(prod_price) as max_price").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // min
    sql = "select min(prod_price) as min_price from products";
    rowresult = products.select("min(prod_price) as min_price").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // sum
    sql = "select sum(quantity) as items_ordered from orderitems where order_num =20005";
    rowresult = orderitems.select("sum(quantity) as items_ordered").where("order_num=20005").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // sum
    sql = "select sum(item_price*quantity) as total_price from orderitems where order_num =20005";
    rowresult = orderitems.select("sum(item_price*quantity) as total_price").where("order_num=20005").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // avg distinct 去重复
    sql = "select avg(distinct prod_price) as avg_price from products where vend_id=1003";
    rowresult = sess.sql(sql).execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // count
    sql = "select count(*) as num_prods from products where vend_id=1003";
    rowresult = products.select("count(*) as num_prods").where("vend_id=1003").execute();
    cout << "#11-2-" << sql << endl << rowresult << endl;

    // groud by
    sql = "select vend_id, count(*) as num_prods from products groud by vend_id";
    rowresult = products.select("vend_id", "count(*) as num_prods").groupBy("vend_id").execute();
    cout << "#13-2-" << sql << endl << rowresult << endl;

    // groud by having
    sql = "select cust_id, count(*) as orders from orders group by cust_id having count(*)>=2";
    rowresult = orders.select("cust_id", "count(*) as orders").groupBy("cust_id").having("count(*)>=2").execute();
    cout << "#13-2-" << sql << endl << rowresult << endl;

    // groud by having
    sql =
        "select vend_id, count(*) as num_prods from products "
        "where prod_price>=10 group by vend_id having(count(*)>=2)";
    rowresult = products.select("vend_id", "count(*) as num_prods")
                    .where("prod_price>=10")
                    .groupBy("vend_id")
                    .having("count(*)>=2")
                    .execute();
    cout << "#13-3-" << sql << endl << rowresult << endl;

    // order by
    sql =
        "select order_num, sum(quantity*item_price) as ordertotal from orderitems "
        "group by order_num having ordertotal>=50 order by ordertotal";
    rowresult = orderitems.select("order_num", "sum(quantity*item_price) as ordertotal")
                    .groupBy("order_num")
                    .having("ordertotal>=50")
                    .orderBy("ordertotal")
                    .execute();
    cout << "#13-4-" << sql << endl << rowresult << endl;

    // order by
    sql =
        "select order_num, sum(quantity*item_price) as ordertotal from orderitems "
        "group by order_num having ordertotal>=50 order by ordertotal";
    rowresult = orderitems.select("order_num", "sum(quantity*item_price) as ordertotal")
                    .groupBy("order_num")
                    .having("ordertotal>=50")
                    .orderBy("ordertotal")
                    .execute();
    cout << "#13-4-" << sql << endl << rowresult << endl;

    // 执行sql语句
    sql = "select cust_id from orders where order_num in (select order_num from orderitems where prod_id='TNT2')";
    rowresult = sess.sql(
                        "select cust_id from orders where order_num in "
                        "(select order_num from orderitems where prod_id='TNT2')")
                    .execute();
    cout << "#14-2-" << sql << endl << rowresult << endl;

    // insert
    sql =
        "insert into customers value("
        "10006,'Pep E. LaPew', '100 main Sstreet', 'Los Angeles', 'CA', '90046','USA',null,null);";
    Result result = customers
                        .insert("cust_id", "cust_name", "cust_address", "cust_city", "cust_state", "cust_zip",
                                "cust_country", "cust_contact", "cust_email")
                        .values("10006", "Pep E. LaPew", "100 main Sstreet", "Los Angeles", "CA", "90046", "USA",
                                nullvalue, nullvalue)
                        .execute();
    cout << "#19-2-" << sql << endl << result << endl;

    // update
    sql = "update customers set cust_email = 'elmer@fudd.com' where cust_id =10005";
    result = customers.update().set("cust_email", "elmer@fudd.com").where("cust_id =10005").execute();
    cout << "#20-1-" << sql << endl << result << endl;

    // update ...
    sql = "update customers set cust_email = 'elmer@fudd.com' where cust_id =10005";
    result =
        customers.update().set("cust_email", nullvalue).set("cust_name", "E Fudd").where("cust_id =10005").execute();
    cout << "#20-1-" << sql << endl << result << endl;

    // delete
    sql = "delete from customers where cust_id=10006";
    result = customers.remove().where("cust_id=10006").execute();
    cout << "#20-2-" << sql << endl << result << endl;

    sess.close();
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
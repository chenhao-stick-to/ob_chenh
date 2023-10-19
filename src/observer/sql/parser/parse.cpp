/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi
//

#include <mutex>
#include "sql/parser/parse.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"

RC parse(char *st, ParsedSqlNode *sqln);

CalcSqlNode::~CalcSqlNode()
{
  for (Expression *expr : expressions) {
    delete expr;
  }
  expressions.clear();
}

ParsedSqlNode::ParsedSqlNode() : flag(SCF_ERROR)
{}

ParsedSqlNode::ParsedSqlNode(SqlCommandFlag _flag) : flag(_flag)
{}

void ParsedSqlResult::add_sql_node(std::unique_ptr<ParsedSqlNode> sql_node)
{
  sql_nodes_.emplace_back(std::move(sql_node));
}

////////////////////////////////////////////////////////////////////////////////

int sql_parse(const char *st, ParsedSqlResult *sql_result);

// RC check_date(int y, int m, int d)
// {
//     static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//     bool leap = (y%400==0 || (y%100 && y%4==0));
//     if(y > 0
//         && (m > 0)&&(m <= 12)
//         && (d > 0)&&(d <= ((m==2 && leap)?1:0) + mon[m])){
//           return RC::SUCCESS;
//         }
//     return RC::INVALID_ARGUMENT;
// }

RC parse(const char *st, ParsedSqlResult *sql_result)
{
  sql_parse(st, sql_result);
  
  //z这个可以在后面的stmt做，也可以在这里做，在这里做需要对sql_result数据结构进行解析以及分析；后面因为resolve自带，所以不需要做，只是代码不易理解
  // std::string s_tmp=s;
  // int y=atoi(s_tmp.substr(1,s_tmp.find_first_of("-")).c_str());

  // std::string ss_tmp=s_tmp.substr(s_tmp.find_first_of("-")+1);
  // std::string ss_tmp2=ss_tmp.substr(0,ss_tmp.find_first_of("-"));
  // if((ss_tmp2.substr(0,1))=="0")
  //   ss_tmp2=ss_tmp2.substr(1);
  // int m=atoi(ss_tmp2.c_str());

  // std::string sss_tmp=ss_tmp.substr(ss_tmp.find_first_of("-")+1);
  // std::string sss_tmp2=sss_tmp.substr(0,strlen(sss_tmp.c_str())-1);
  
  // if((sss_tmp2.substr(0,1))=="0")
  //   sss_tmp2=sss_tmp2.substr(1);
  // int d=atoi(sss_tmp2.c_str());

  // RC rc = check_date(y,m,d);
  // if (rc != RC::SUCCESS && rc != RC::UNIMPLENMENT) {
  // LOG_WARN("failed to parse argument date value. rc=%d:%s", rc, strrc(rc));
  // sql_result->set_return_code(rc);
  // return rc;
  // }
  return RC::SUCCESS;
}

bool check_date(const char * data)
{
  std::string s_tmp=data;
  int y=atoi(s_tmp.substr(1,s_tmp.find_first_of("-")).c_str());

  std::string ss_tmp=s_tmp.substr(s_tmp.find_first_of("-")+1);
  std::string ss_tmp2=ss_tmp.substr(0,ss_tmp.find_first_of("-"));
  if((ss_tmp2.substr(0,1))=="0")
    ss_tmp2=ss_tmp2.substr(1);
  int m=atoi(ss_tmp2.c_str());

  std::string sss_tmp=ss_tmp.substr(ss_tmp.find_first_of("-")+1);
  std::string sss_tmp2=sss_tmp.substr(0,strlen(sss_tmp.c_str())-1);
  
  if((sss_tmp2.substr(0,1))=="0")
    sss_tmp2=sss_tmp2.substr(1);
  int d=atoi(sss_tmp2.c_str());
  
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool leap = (y%400==0 || (y%100 && y%4==0));
  return  (y > 0
      && (m > 0)&&(m <= 12)
      && (d > 0)&&(d <= ((m==2 && leap)?1:0) + mon[m]));
}
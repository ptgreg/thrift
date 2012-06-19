#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>
#include "t_generator.h"
#include "platform.h"
using namespace std;

class Singleton 
{
public:
  Singleton(){
    f_out_.open("thriftGraph.dot");
    f_out_ << "digraph G {" << endl;
  f_out_ << "fontname = \"Bitstream Vera Sans\"" << endl;
  f_out_ << "fontsize = 8" << endl;
  f_out_ << endl;
  f_out_ << "node [" << endl;
  f_out_ << "fontname = \"Bitstream Vera Sans\"" << endl;
  f_out_ << "fontsize = 8" << endl;
  f_out_ << "shape = \"record\"" << endl;
  f_out_ << "]" << endl;
  f_out_ << endl;
  f_out_ << "edge [" << endl;
  f_out_ << "fontname = \"Bitstream Vera Sans\"" << endl;
  f_out_ << "fontsize = 8" << endl;
  f_out_ << "arrowhead = \"empty\"" << endl;
  f_out_ << "]" << endl;
  f_out_ << endl;
  }

  ~Singleton(){
    f_out_ << "}" << endl;
    f_out_.close();
  }

  std::ofstream f_out_;
};

class t_html_generator : public t_generator
{
 public:
  static Singleton file;


	t_html_generator(
		t_program* program,
		const std::map<std::string, std::string>& parsed_options,
		const std::string& option_string)
		: t_generator(program)
	{
		(void) parsed_options;
		(void) option_string;
		out_dir_base_ = "gen-dot";
		cout << "Calling constructor:" << option_string << endl;
		for(std::map<std::string, std::string>::const_iterator i = parsed_options.begin(); i != parsed_options.end(); ++i)
		  {
		    cout << "k: " << (*i).first << " v: " << (*i).second << endl;
		  }
		
	}

	void init_generator();
	void close_generator();

	void generate_typedef  (t_typedef*  ttypedef);
	void generate_enum     (t_enum*     tenum);
	void generate_struct   (t_struct*   tstruct);
	void generate_xception (t_struct*   txception);
	void generate_service  (t_service*  tservice);

	string type_name(t_type* ttype);
  set<string> find_dependency(t_type* ttype);
	string base_type_name(t_base_type* tbase);

	std::ofstream f_out_;
};

Singleton t_html_generator::file;

void t_html_generator::init_generator() {

  // Make output directory
  
  cout << "call " << program_->get_name() << endl;

}

void t_html_generator::close_generator() {
}

void t_html_generator::generate_typedef(t_typedef*  ttypedef)
{
	(void) ttypedef;
}

void t_html_generator::generate_xception   (t_struct*   tstruct){
	(void)  tstruct;
}

void t_html_generator::generate_service  (t_service*  tservice){
	(void) tservice;
}


void t_html_generator::generate_enum(t_enum*     tenum)
{
  file.f_out_ << endl;
  file.f_out_ << tenum->get_name() << " [" << endl;
	file.f_out_ << "label = \"{ enum " << tenum->get_name() << "|";

	vector<t_enum_value*> consts = tenum->get_constants();
	vector<t_enum_value*>::iterator c_iter;
	for (c_iter = consts.begin(); c_iter != consts.end(); ++c_iter) {
	  file.f_out_ << (*c_iter)->get_name() << " : " << (*c_iter)->get_value() << "\\l";
	}
	file.f_out_ << "}\"";
	file.f_out_ << "]";	
	file.f_out_ << endl;
}

void t_html_generator::generate_struct   (t_struct*   tstruct){
  file.f_out_ << endl;
  file.f_out_ << tstruct->get_name() << " [" << endl;
  file.f_out_ << "label = \"{" <<  tstruct->get_name() << "|";
	
	const vector<t_field*>& members = tstruct->get_members();
    vector<t_field*>::const_iterator m_iter;
  
    
	for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
	  file.f_out_ << (*m_iter)->get_key() << " : " << (*m_iter)->get_name() << " : " << type_name((*m_iter)->get_type()) << "\\l";
	}
	
	file.f_out_ << "}\"" << endl;
	file.f_out_ << "]";
	file.f_out_ << endl;

	set<string> dependenciesSet;
	for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
	  set<string> dependencies = find_dependency((*m_iter)->get_type());
	      for(set<string>::const_iterator iter = dependencies.begin(); iter != dependencies.end(); ++iter)
		{
		  dependenciesSet.insert(*iter);
		}
	}
	for(set<string>::const_iterator iter = dependenciesSet.begin(); iter != dependenciesSet.end(); ++iter){
	  file.f_out_ << tstruct->get_name() << " -> " << (*iter) << endl;
	}
	file.f_out_ << endl;
}

set<string> t_html_generator::find_dependency(t_type* ttype)
{
  set<string> result;
  
  while (ttype->is_typedef()) {
    ttype = ((t_typedef*)ttype)->get_type();
  }

  if (ttype->is_base_type())
    {
      return result;
    }
  else if (ttype->is_map())
    {
      t_map *tmap = (t_map*) ttype;
      set<string> key = find_dependency(tmap->get_key_type());
      set<string> value = find_dependency(tmap->get_val_type());
      for(set<string>::const_iterator iter = key.begin(); iter != key.end(); ++iter)
	{
	    result.insert(*iter);
	}
      for(set<string>::const_iterator iter = value.begin(); iter != value.end(); ++iter)
	{
	    result.insert(*iter);
	}
      return result;
    }
  else if(ttype->is_set())
    {
      t_set* tset = (t_set*) ttype;
      set<string> key = find_dependency(tset->get_elem_type());
      for(set<string>::const_iterator iter = key.begin(); iter != key.end(); ++iter)
	{
	    result.insert(*iter);
	}
      return result;
    }
  else if (ttype->is_list())
    {
      t_list* tlist = (t_list*) ttype;
      set<string> key = find_dependency(tlist->get_elem_type());
      for(set<string>::const_iterator iter = key.begin(); iter != key.end(); ++iter)
	{
	    result.insert(*iter);
	}
      return result;
    }
  result.insert(ttype->get_name());
  return result;
}

string t_html_generator::type_name(t_type* ttype) {  
  while (ttype->is_typedef()) {
    ttype = ((t_typedef*)ttype)->get_type();
  }

  if (ttype->is_base_type()) {
    return base_type_name((t_base_type*)ttype);
  } else if (ttype->is_map()) {
    t_map *tmap = (t_map*) ttype;
    return "Dictionary\\<" + type_name(tmap->get_key_type()) +
      ", " + type_name(tmap->get_val_type()) + "\\>";
  } else if (ttype->is_set()) {
    t_set* tset = (t_set*) ttype;
    return "Set\\<" + type_name(tset->get_elem_type()) + "\\>";
  } else if (ttype->is_list()) {
    t_list* tlist = (t_list*) ttype;
    return "List\\<" + type_name(tlist->get_elem_type()) + "\\>";
  }
  
  return ttype->get_name();
}

string t_html_generator::base_type_name(t_base_type* tbase) {
  switch (tbase->get_base()) {
    case t_base_type::TYPE_VOID:
      return "void";
    case t_base_type::TYPE_STRING:
      if (tbase->is_binary()) {
        return "byte[]";
      } else {
        return "string";
      }
    case t_base_type::TYPE_BOOL:
      return "bool";
    case t_base_type::TYPE_BYTE:
      return "byte";
    case t_base_type::TYPE_I16:
      return "short";
    case t_base_type::TYPE_I32:
      return "int";
    case t_base_type::TYPE_I64:
      return "long";
    case t_base_type::TYPE_DOUBLE:
      return "double";
    default:
      throw "compiler error: no  name for base type " + tbase->get_base();
  }
}

THRIFT_REGISTER_GENERATOR(html, "HTML", "")

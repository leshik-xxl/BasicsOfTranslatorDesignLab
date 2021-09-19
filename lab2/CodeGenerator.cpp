//
// Created by oleks on 26.05.2021.
//

#include "CodeGenerator.h"

#include <cstdarg>
#include <algorithm>

void CodeGenerator::addLine(const string& line, int newIndent)
{
   if (newIndent != -1) indent = newIndent;
   for (int i = 0; i < indent; i++) code += " ";
   code += line + "\n";
}

string CodeGenerator::makeCode()
{
   return " .486\n"
          ".model flat, stdcall\n"
          ".code\n"
          "\n"
          + code +
          "\n"
          "END\n";
}

void CodeGenerator::printError(const Node& node, string format, ...)
{
   char buff[300];

   va_list argp;
   va_start(argp, format);
   vsnprintf(buff, sizeof(buff), format.c_str(), argp);
   va_end(argp);

   Node terminal = node.searchAnyTerminal();
   if (terminal.isEmpty())
      output << "Code generator error: ";
   else
      output << "Code generator error at line " << terminal.lexeme.line << ", column " << terminal.lexeme.column << ": ";

   output << (const char*) buff << endl;
}

bool in_str_vector(const vector<string>& v, const string& str)
{
   return find(v.begin(), v.end(), str) != v.end();
}

bool CodeGenerator::collectVariables()
{
   bool has_errors = false;
   Node list = root.search("parameters_list");
   if (list.isEmpty()) return has_errors;

   vector<Node> declarations = root
         .search("declarations_list")
         .extractList("declaration", "declarations_list");

   for (const auto& decl : declarations)
   {
      vector<Node> attributes = decl.extractList("attribute", "attributes_list");
      string base_attribute;
      vector<string> add_attributes;
      for (const auto& var : attributes)
      {
         string name = var.getKw(table);
         if (name == "INTEGER" || name == "FLOAT" || name == "BLOCKFLOAT")
         {
            if (!base_attribute.empty())
            {
               has_errors = true;
               printError(var, "Specified %s two or more base attributes specified at the same time", name.c_str());
            }
            else
            {
               base_attribute = name;
            }
         }
         else if (name == "EXT" || name == "SIGNAL" || name == "COMPLEX")
         {
            if (in_str_vector(add_attributes, name))
            {
               has_errors = true;
               printError(var, "Base attribute %s already declared", name.c_str());
            }
            else
            {
               add_attributes.push_back(name);
            }
         }
         else
         {
            has_errors = true;
            printError(decl, "Unknown attribute %s", name.c_str());
            continue;
         }
      }

      vector<Node> vars = decl.extractList("variable_identifier", "identifiers_list");
      for (const auto& var : vars)
      {
         string variable_name = var.getId(table);
         bool found = false;
         for(const auto& j : variables)
            if(j.name == variable_name) found = true;

         if(found)
         {
            has_errors = true;
            printError(var, "Variable with name %s was already declared", variable_name.c_str());
            continue;
         }

         variable ve = variable(variable_name, var.lexeme);
         ve.base_type = base_attribute;
         ve.attributes = add_attributes;
         variables.emplace_back(ve);
      }
   }

   return has_errors;
}

string CodeGenerator::getProgramName()
{
   return root.search("procedure_identifier").getId(table);
}


bool CodeGenerator::makeMainProcedure()
{
   bool has_errors = false;
   string name = getProgramName();
   addLine("PUBLIC " + name, DECL_INDENT);
   addLine("");
   addLine(";; Main procedure declaration");

   string parameters;
   for(int i = 0; i < variables.size(); i++)
   {
      parameters += variables[i].name + ": ";
      if(variables[i].base_type == "FLOAT")
         parameters += "FLOAT";
      else if(variables[i].base_type == "BLOCKFLOAT")
         parameters += "BLOCKFLOAT";
      else if(variables[i].base_type == "INTEGER")
         parameters += "DWORD";
      else
      {
         has_errors = true;
         printError(Node(), "Unknown attribute %s", name.c_str());
         continue;
      }
      if(i != variables.size() -1)
         parameters += ", ";
   }

   addLine(name + " PROC NEAR " + parameters, DECL_INDENT);
   addLine("PUSH BP", BODY_INDENT);
   addLine("MOV BP, SP", BODY_INDENT);
   addLine("POP BP", BODY_INDENT);
   addLine("RET", BODY_INDENT);
   addLine(name + " ENDP", DECL_INDENT);

   return has_errors;
}

bool CodeGenerator::startCode()
{
   bool has_errors = false;

   has_errors = collectVariables();
   has_errors = has_errors || makeMainProcedure();

   code = makeCode();
   return !has_errors;
}

void CodeGenerator::printCode() const
{
   output << code;
}


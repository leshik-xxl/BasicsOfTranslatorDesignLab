//
// Created by oleks on 27.04.2021.
//

#include "SyntacticalAnalyzer.h"

string SyntacticalAnalyzer::createError(string format, ...) {
    char buff[300];

    va_list argp;
    va_start(argp, format);
    vsnprintf(buff, sizeof(buff), format.c_str(), argp);
    va_end(argp);

    PropertyLocation p = lexemes[lex_counter];
    string result =
            "SyntacticalAnalyzer: Error ( line: " + to_string(p.line) + ", column " + to_string(p.column) + "): ";
    result += (const char *) buff;
    return result;
}

void SyntacticalAnalyzer::appendTerminal(Node &n, const string &name, PropertyLocation lex) {
    Node node(name, lex);
    node.terminal = true;
    n.nodes.push_back(node);
}

SyntacticalAnalyzer::read_res SyntacticalAnalyzer::readKeyword(Node &n, const string &keyword) {
    IdType type = table.classifyIndex(lexemes[lex_counter].id);
    string expected = keyword;
    if(keyword == "PROCEDURE"){
        expected = "PROGRAM | PROCEDURE";
    }
    if(keyword == "EXT"){
        expected = "SIGNAL | COMPLEX | INTEGER | FLOAT | BLOCKFLOAT | EXT";
    }
    if (type != IdType::Keyword) {
        string error = createError("'%s' keyword lexeme expected, but %s found", expected.c_str(), idTypeToString(type).c_str());
        read_res readRes;
        readRes.ok = false;
        readRes.error = error;
        return readRes;
    }
    string key;
    if ((key = table.getKeyword(lexemes[lex_counter].id)) != keyword) {
        string error = createError("'%s' keyword expected, but %s keyword found", expected.c_str(), key.c_str());
        read_res readRes;
        readRes.ok = false;
        readRes.error = error;
        return readRes;
    }
    appendTerminal(n, "keywords", lexemes[lex_counter++]);
    return {
            .ok = true,
    };
}

SyntacticalAnalyzer::read_res SyntacticalAnalyzer::readDm(Node &n, const char delimiter) {
    IdType type = table.classifyIndex(lexemes[lex_counter].id);
    if (type != IdType::DM) {
        string error = createError("'%c' delimiter lexeme expected, but %s found",(char) delimiter, idTypeToString(type).c_str());
        read_res readRes;
        readRes.ok = false;
        readRes.error = error;
        return readRes;
    }
    if (lexemes[lex_counter].id != delimiter) {
        string error = createError("'%c' delimiter expected, but %c delimiter found", (char) delimiter,
                                   (char) lexemes[lex_counter].id);
        read_res readRes;
        readRes.ok = false;
        readRes.error = error;
        return readRes;
    }
    appendTerminal(n, "delimiter", lexemes[lex_counter++]);
    return {
            .ok = true,
    };
}


SyntacticalAnalyzer::read_res SyntacticalAnalyzer::readIdentifier(Node &n) {
    IdType type = table.classifyIndex(lexemes[lex_counter].id);
    if (type != IdType::Id) {
        string error = createError("Identifier lexeme expected, but %s found", idTypeToString(type).c_str());
        read_res readRes;
        readRes.ok = false;
        readRes.error = error;
        return readRes;
    }
    appendTerminal(n, "identifier", lexemes[lex_counter++]);
    return {
            .ok = true,
    };
}


#define DECL(name) SyntacticalAnalyzer::read_res SyntacticalAnalyzer::name##_func() { Node node(#name, lexemes[lex_counter]); read_res rr; int old = lex_counter;
#define ENDDECL return { .ok = true, .data = node }; }
#define READ(expr) if(!(rr = (expr)).ok) { return rr; }
#define READP(expr) READ(expr) else node.nodes.push_back(rr.data);
#define FALLBACK { node.nodes.clear(); lex_counter = old; }

DECL(root)
    {
        return signal_program_func();
    }
ENDDECL

DECL(signal_program)
    {
        READP(program_func());
    }
ENDDECL

DECL(program)
    {
        if (!readKeyword(node, "PROGRAM").ok) {
            FALLBACK;
            READ(readKeyword(node, "PROCEDURE"));
            READP(procedure_identifier_func());
            READP(parametrs_list_func());
            READ(readDm(node, ';'));
            READP(block_func());
            READ(readDm(node, ';'));
        } else {
            READP(procedure_identifier_func());
            READ(readDm(node, ';'));
            READP(block_func());
            READ(readDm(node, '.'));
        }

    }
ENDDECL

DECL(block)
    {
        READ(readKeyword(node, "BEGIN"));
        READP(statement_list_func());
        READ(readKeyword(node, "END"));
    }
ENDDECL

DECL(statement_list)
    {
        if ((rr = statement_func()).ok) {
            node.nodes.push_back(rr.data);
            READP(statement_list_func());
        } else {
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL

DECL(statement)
    {

         if(!readDm(node, ';').ok){
             FALLBACK
             if(readKeyword(node, "RETURN").ok){
                 READ(readDm(node, ';'));
             }else{
                 FALLBACK;
                 READP(condition_statement_func());
                 READ(readKeyword(node, "ENDIF"));
                 READ(readDm(node, ';'));
             }
         }

    }
ENDDECL

DECL(condition_statement)
    {
        READ(readKeyword(node, "IF"));
        READP(condition_expression_func());
        READ(readKeyword(node, "THEN"));
        READP(statement_list_func());
        READP(alternative_part_func());
    }
ENDDECL

DECL(alternative_part)
    {
        if(readKeyword(node, "ELSE").ok){
            READP(statement_list_func());
        } else{
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL

DECL(condition_expression)
    {
        READP(identifier_func());
    }
ENDDECL


DECL(parametrs_list)
    {
        if (readDm(node, '(').ok) {
            READP(declarations_list_func());
            READ(readDm(node, ')'));
        } else {
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL

DECL(declarations_list)
    {
        if ((rr = declaration_func()).ok) {
            node.nodes.push_back(rr.data);
            READP(declarations_list_func());
        } else {
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL

DECL(declaration)
    {
        READP(variable_identifier_func());
        READP(identifiers_list_func());
        READ(readDm(node, ':'));
        READP(attribute_func());
        READP(attributes_list_func());
        READ(readDm(node, ';'))
    }
ENDDECL

DECL(identifiers_list)
    {
        if (readDm(node, ',').ok) {
            READP(variable_identifier_func());
            READP(identifiers_list_func());
        } else {
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL

DECL(attributes_list)
    {
        if ((rr = attribute_func()).ok) {
            node.nodes.push_back(rr.data);
            READP(attributes_list_func());
        } else {
            FALLBACK;
            node.markEmpty();
        }
    }
ENDDECL


DECL(attribute)
    {
        if (!readKeyword(node, "SIGNAL").ok) {
            FALLBACK;
            if (!readKeyword(node, "COMPLEX").ok) {
                FALLBACK;
                if (!readKeyword(node, "INTEGER").ok) {
                    FALLBACK;
                    if (!readKeyword(node, "BLOCKFLOAT").ok) {
                        FALLBACK;
                        if (!readKeyword(node, "FLOAT").ok) {
                            FALLBACK;
                            READ(readKeyword(node, "EXT"));
                        }
                    }
                }
            }
        }
    }
ENDDECL


DECL(variable_identifier)
    {
        READP(identifier_func());
    }
ENDDECL

DECL(procedure_identifier)
    {
        READP(identifier_func());
    }
ENDDECL

DECL(identifier)
    {
        READ(readIdentifier(node));
    }
ENDDECL


#undef DECL
#undef ENDDECL

bool SyntacticalAnalyzer::startSyntacticalAnalyzer() {
    read_res rr = root_func();
    if (!rr.ok) {
        output << rr.error;
    } else {
        rootNode = rr.data;
    }
    return rr.ok;
}

void SyntacticalAnalyzer::printNodes() {
    rootNode.print(table, output);
}

void SyntacticalAnalyzer::reset() {
    lex_counter = 0;
}
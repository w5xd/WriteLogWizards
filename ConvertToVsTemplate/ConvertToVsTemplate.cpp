// ConvertToVsTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This program is a utility used one time to help convert the old vsz style WriteLog module wizard to use the VSIX tools.
// It parses the old-style WriteLog template files and replaces their [!if ] and [!output etc with $if$ etc for the vstemplate style
// wizard
// 
// This conversion logic is based on: https://stackoverflow.com/questions/6709057/visual-studio-project-item-template-parameter-logic
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>

static int ProcessFile(std::istream &in, std::ostream &out, std::set<std::string> &outputSymbols, 
    std::set<std::string> &ifSymbols, std::set<std::string> & compositIfs);

static bool showOrigIf = false;
int main(int argc, char *argv[])
{
    std::string f1;
    std::string f2;
    std::string symbols;
    std::string csName;

    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "-s")
        {
            if (++i < argc)
                symbols = argv[i];
            else
                break;
        }
        else if (arg == "-cs")
            csName = "CompositeSymbols.cs";
        else if (arg == "-d")
            showOrigIf = true;
        else if (f1.empty())
            f1 = argv[i];
        else if (f2.empty())
            f2 = argv[i];
        else
        {
            f1.clear();
            break;
        }
    }


    if (csName.empty() && (f1.empty() || f2.empty()))
    {
        std::cerr << "Usage: ConvertToVsTemplate [<input-file-name> <output-file-name>] [-s <symbol-file-name> [-cs]]  [-d]" << std::endl;
        return 1;
    }

    std::ifstream inFile;
    if (!f1.empty() && (inFile.open(f1), !inFile.is_open()))
    {
        std::cerr << "Failed to open input " << f1 << std::endl;
        return 1;
    }
    std::ofstream outFile;
    if (inFile.is_open() && !f2.empty() && (outFile.open(f2), !outFile.is_open()))
    {
        std::cerr << "Failed to open output " << f2 << std::endl;
        return 1;
    }
    if (outFile.is_open())
    {
        outFile << "$if$ (0) /*Copyright (c) 2025 by Wayne E. Wright, W5XD" << std::endl;
        outFile << "This template was converted from the version used for the old vsz template technology." << std::endl;
        outFile << "The old style used [!if ]. The new one uses the \"if\" that commands the processor to skip this very commentary." << std::endl;
        outFile << "The difference between the two is (a) that the old tech supported nested \"if\" clauses while the new one does not," << std::endl;
        outFile << "and (b) the old style supported && and || operators and the new one only supports string compare for \"if\" clauses." << std::endl;
        outFile << "The way I made this work was to extract all the compounded conditional expressions that in the old tech were" << std::endl  ;
        outFile << "evaluated by the template processor, and make the wizard here evaluate the expressions and write a separate" << std::endl ;
        outFile << "for each expression.*/";
        outFile << "$endif$";
    }
    std::ofstream csFile;
    if (!csName.empty() && (csFile.open(csName), !csFile.is_open()))
    {
        std::cerr << "Failed to write " << csName << std::endl;
        return 1;
    }

    std::set<std::string> outputSymbols;
    std::set<std::string> ifSymbols;
    std::set<std::string> compositIfs;

static const char OUTPUT_TAG[] = "****output_symbols****";
static const char CONDITIONAL_TAG[] = "****conditional_symbols****";
static const char CCOMPOSITES_TAG[] = "**** Composites ****";

    if (!symbols.empty())
    {
        std::ifstream sy(symbols);
        if (sy.is_open())
        {
            int state = 0;
            std::string line;
            while (std::getline(sy, line))
            {
                for (;;)
                {
                    auto dollar = line.find('$');
                    if (dollar == line.npos)
                        break;
                    line.erase(line.begin() + dollar);
                }
                switch (state)
                {
                    case 0:
                        if (line == OUTPUT_TAG)
                            state += 1;
                        break;
                    case 1:
                        if (line == CONDITIONAL_TAG)
                            state += 1;
                        else if (!line.empty())
                            outputSymbols.insert(line);
                        break;
                    case 2:
                        if (line == CCOMPOSITES_TAG)
                            state += 1;
                        else if (!line.empty())
                            ifSymbols.insert(line);
                        break;
                    case 3:
                        if (!line.empty())
                            compositIfs.insert(line);
                        break;
                }
            }
        }

    }

    int r = 0;
    if (inFile.is_open() && outFile.is_open())
        r = ProcessFile(inFile, outFile, outputSymbols, ifSymbols, compositIfs);

    if (!symbols.empty())
    {
        std::ofstream outS(symbols);
        if (!outS.is_open())
        {
            std::cerr << "Failed to open symbol file to write: " << symbols << std::endl;
            return 0;
        }
        outS << OUTPUT_TAG << std::endl;
        for (const auto &o : outputSymbols)
            outS << '$' << o << '$' << std::endl;

        outS << CONDITIONAL_TAG << std::endl;
        for (const auto& o : ifSymbols)
            outS << '$' << o << '$' << std::endl;

        outS << CCOMPOSITES_TAG << std::endl;
        for (const auto& o : compositIfs)
            outS << o << std::endl;
    }

    if (csFile.is_open())
    {
        csFile << "using System.Collections.ObjectModel;\n"
                "// This file is prgrammatically generated by ConvertToVsTemplate based on scanning the input template files\n"
                "namespace WL12ModuleItem.CppTemplates.WL12ModuleTemplate\n"
                "{\n"
                "     public class CompositeSymbols {\n"
                "         public static readonly ReadOnlyCollection<string> conditionArray = new ReadOnlyCollection<string>(new string[] {"
                << std::endl;
        for (const auto& o : compositIfs)
            csFile << "        \"" <<  o << "\"," << std::endl;
        csFile << "        });\n"
                  "    }\n"
                "}" << std::endl;
    }

    return r;
}

class VszIf {
    public:
        VszIf(const std::string &arg) : m_arg(arg), m_amElseing(false), m_allOpsMatch(true), m_isOr(false)
        {
            std::string a;
            std::string op;
            bool neg(false);
            int state = 0;
            for (auto c: arg)
            {
                auto spc = isspace(c);
                auto isSymbol = isdigit(c) || isalpha(c) || (c == '_');
                if (c == '!' && state == 2)
                    state = 3;
                switch(state)
                {
                    case 3: // operator complete
                        if (m_allOps.empty())
                            m_allOps = op;
                        if (m_allOps != op)
                            m_allOpsMatch = false;
                        if (!a.empty())
                            m_args.push_back(Arg(neg, a, op));
                        neg = false;
                        a.clear();
                        op.clear();
                        state = 0;
                        // fall through
                    case 0: // next arg
                        if (spc)
                            state += 1;
                        else if (c == '!')
                            neg = true;
                        else if (isSymbol)
                            a += c;
                        else
                        {
                            state += 1;
                            goto more_arg;
                        }
                        break;
                    case 1: //arg complete
                        more_arg:
                        if (spc)
                            break;
                        else
                            state +=1;
                            // fall through
                    case 2: // operator
                        if (spc)
                        {
                            state += 1;
                            break;
                        }
                        op += c;
                        break;
                }
            }
            if (!a.empty())
                m_args.push_back(Arg(neg, a, op));
            m_isOr = m_allOps == "||";
        }

        void renderSymbols(std::set<std::string> &symbols, std::set<std::string>& compositIfs)
        {
            std::string composite;
            bool isComposite = false;
            for (const auto &a : m_args)
            {
                if (!composite.empty())
                    isComposite = true;
                symbols.insert(a.arg);
                if (a.negated)
                    composite += '!';
                composite += a.arg;
                composite += a.op;
            }
            if (isComposite)
                compositIfs.insert(composite);
        }

        // This is complicated bacause the VC++ template processor cannot
        // process nested $if$ while the old style vsz did handle nested [!if
        // AND there are no logical operators || or && in 
        std::string onStartIF(std::shared_ptr<VszIf> inForce)
        {
            m_inForce = inForce;
            std::string r;
            if (!m_allOpsMatch)
            {
                r += "Oops all ops don't match";
                std::cerr << r << std::endl;
            }
            if (!m_allOps.empty() && m_allOps != "&&" && !m_isOr)
            {
                r += "Oops. Don't know how to process operator " + m_allOps;
                std::cerr << r << std::endl;
            }
            if (m_inForce)
                r += "$endif$";
            r += ifNow();
            return r;
        }

        std::string onElse()
        {
            m_amElseing = true;
            std::string r;
            r += "$endif$";
            r += ifNow();
            return r;
        }

        std::string onEndif()
        {
            std::string r;
            r += "$endif$";
            if (m_inForce)
                r += m_inForce->ifNow();
            return r;
        }

    protected:
        struct Arg {
            bool negated;
            std::string arg;
            std::string op;
            Arg(bool neg, const std::string &arg, const std::string &op) : arg(arg), negated(neg), op(op)
            {}
        };

        std::string allArgsInForce(bool composing = false)
        {
            std::string r;
            r += argMatch(composing);
            for (auto p = m_inForce; p; p = p->m_inForce)
                r += p->argMatch(composing);
            return r;
        }

        std::string allValuesForIf(bool composing = false)
        {
            std::string r;
            bool negAlone = false;
            if (!composing && m_args.size() == 1)
                negAlone = m_args.begin()->negated;
            r += m_amElseing ^ negAlone ? "0" : "1";
            auto p = m_inForce;
            if (p)
                r += p->allValuesForIf(composing);
            return r;
        }

        std::string ifNow()
        {
            std::string r;
            bool composing = m_args.size() > 1;
            r += "$if$ (";
            r += allArgsInForce();
            r += " == ";
            r += allValuesForIf();
            r += ')';
            return r;
        }

        std::string argMatch(bool composing = false)
        {
            std::string r;
             {
                r += '$';
                for (auto& a : m_args)
                {
                    if (a.negated && (composing || m_args.size() > 1))
                        r += '!';
                    r += a.arg;
                    r += a.op;
                }
                r += '$';
            }
            return r;
        }

        std::shared_ptr<VszIf> m_inForce;
        const std::string m_arg;
        std::vector<Arg> m_args;
        bool m_amElseing;
        bool m_allOpsMatch;
        std::string m_allOps;
        bool m_isOr;
};

static int ProcessFile(std::istream& in, std::ostream& out, 
    std::set<std::string>& outputSymbols, std::set<std::string>& ifSymbols,
    std::set<std::string>& compositIfs)
{
    std::string line;
    std::vector<std::shared_ptr< VszIf>> ifStack;
    while (std::getline(in, line))
    {
        auto stdafx = line.find("stdafx.h");
        if (stdafx != line.npos)
            line = line.substr(0, stdafx) + "pch.h" + line.substr(stdafx+8);
        std::string procLine;
        std::string::size_type pos;
        bool wroteOne = false;
        while (pos = line.find("[!"), pos != line.npos)
        {
            if (pos != 0)
                procLine += line.substr(0,pos);
            std::string op;
            std::string arg;
            auto i = pos+2;
            int state = 0;
            for (; line[i] != ']'; i+=1)
            {
                auto spc = isspace(line[i]);
                switch (state)
                {
                    case 0:
                        if (!spc)
                            op += line[i];
                        else
                            state += 1;
                        break;
                    case 1:
                        if (spc)
                            break;
                        state += 1;
                        // fall through
                    case 2:
                        arg += line[i];
                        break;

                }
            }
            if (op == "output")
            {
                outputSymbols.insert(arg);
                procLine += "$" + arg + "$";
            }
            else if (op == "if")
            {
                auto ifOp = std::make_shared<VszIf>(arg);
                if (!procLine.empty())
                    out << procLine;
                if (!wroteOne && showOrigIf)
                    out << "//" << line << std::endl;
                ifOp->renderSymbols(ifSymbols, compositIfs);
                procLine = ifOp->onStartIF(ifStack.empty() ? std::shared_ptr<VszIf>() : ifStack.back());
                if (!procLine.empty())
                    out << procLine ;
                wroteOne = true;
                procLine.clear();
                ifStack.push_back(ifOp);
            }
            else if (op == "else")
            {
                if (!procLine.empty())
                    out << procLine ;
                if (showOrigIf)
                    out << "//" << line << std::endl;
                procLine = ifStack.back()->onElse();
                if (!procLine.empty())
                    out << procLine;
                wroteOne = true;
                procLine.clear();
            }
            else if (op == "endif")
            {
                if (!procLine.empty())
                    out << procLine ;
                if (showOrigIf)
                    out << "//" << line << std::endl;
                procLine = ifStack.back()->onEndif();
                ifStack.pop_back();
                if (!procLine.empty())
                    out << procLine ;
                wroteOne = true;
                procLine.clear();
            }
            else
                std::cerr << "Unknown operator [!" << op << std::endl;
            line = line.substr(i+1);
        }
        procLine += line;
        if (!procLine.empty() || !wroteOne)
            out << procLine << std::endl;
    }
    return 0;
}



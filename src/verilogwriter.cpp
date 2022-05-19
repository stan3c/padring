/*
    PADRING -- a padring generator for ASICs.

    Copyright (c) 2019, Niels Moseley <niels@symbioticeda.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
    
*/

#include <sstream>
#include <fstream>
#include <iomanip>
#include <complex>
#include <math.h>
#include <assert.h>
#include "logging.h"
#include "verilogwriter.h"

VerilogWriter::VerilogWriter(std::ostream &os)
    : m_def(os)
{
}

VerilogWriter::~VerilogWriter()
{
    m_def.flush();    
    writeToFile();
}

void VerilogWriter::writeToFile()
{
    assert(!m_designName.empty());

    m_def << "`timescale 1ps/1ps\n";
    m_def << "module " << m_designName << " (\n";

    m_def << m_ss_header.str();
    
    m_def << ");\n\n";
    
    m_def << "// Direction phase \n";
    m_def << m_ss_dirs.str();
    
    m_def << "\n\n";

    m_def << "// Variable phase \n";
    m_def << m_ss_vars.str();

    m_def << "// Instantiation phase \n";
    m_def << m_ss_body.str();

    m_def << "endmodule\n";
}

void VerilogWriter::writeCell(const LayoutItem *item)
{
    // First, do the instantiation
    m_ss_body << "  " << item->m_cellname << " " << item->m_instance << "(";
    
    // First, get all the pins, and write header and vars
    bool first = true;
    for(auto it: item->m_lefinfo->m_pins) {
        // Avoid all non-signal
        if(it.second->m_use != 0) continue;
        
        // The var name
        std::string varName = item->m_instance + "_" + it.first;
        
        // Put it in the dirs
        m_ss_dirs << "  ";
        if(it.second->m_dir == 0) m_ss_dirs << "input ";
        if(it.second->m_dir == 1) m_ss_dirs << "output ";
        if(it.second->m_dir == 2) m_ss_dirs << "inout ";
        m_ss_dirs << varName << ";\n";
        
        // Put it in the vars
        m_ss_vars << "  wire " << varName << ";\n";
        
        // Put it in the body
        if(!first) m_ss_body << ", ";
        m_ss_body << "." << it.first << "(" << varName << ")";
        first = false;
    }
    
    // Close the current instantiation
    m_ss_body << ");\n";
}

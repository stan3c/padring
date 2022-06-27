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
#include "csvwriter.h"

CSVWriter::CSVWriter(std::ostream &os)
    : m_def(os), m_side(0)
{
}

CSVWriter::~CSVWriter()
{
    m_def.flush();    
    writeToFile();
}

void CSVWriter::writeToFile()
{
    m_def << "Back to Index,,,,,,,,,,,,,,,,,,,,,,\n";
    m_def << ",Pin Assignment (R4252),,,,,,,,,,,,,,,,,,,,,\n";
    m_def << ",,,,,,,,,,,,,,,,,,,,,,";
    m_def << ",,,,,Pin Name,,,,,,,,,,,,,,,,,";
    m_def << ",,Pin No.,Pin Assign,Symbol/Layout content,Original,I/O name,Note,,,,,,,,,,,,,,,";

    m_def << m_ss.str();
}

void CSVWriter::writePadring(PadringDB *padring)
{
    // Enumerate
    for(auto item : padring->m_south)
    {
        if (item->m_ltype == LayoutItem::TYPE_CELL)
        {
            m_side++;
            m_ss << ",SOUTH," << m_side << ",I/O,NONE," << item->m_instance << ","<< item->m_cellname << ",,,,,,,,,,,,,,,,\n";
        }
    }
    for(auto item : padring->m_east)
    {
        if (item->m_ltype == LayoutItem::TYPE_CELL)
        {
            m_side++;
            m_ss << ",EAST," << m_side << ",I/O,NONE," << item->m_instance << ","<< item->m_cellname << ",,,,,,,,,,,,,,,,\n";
        }
    }
    for (auto item = padring->m_north.rbegin(); item != padring->m_north.rend(); ++item)
    {
        if ((*item)->m_ltype == LayoutItem::TYPE_CELL)
        {
            m_side++;
            m_ss << ",NORTH," << m_side << ",I/O,NONE," << (*item)->m_instance << ","<< (*item)->m_cellname << ",,,,,,,,,,,,,,,,\n";
        }
    }
    for (auto item = padring->m_west.rbegin(); item != padring->m_west.rend(); ++item)
    {
        if ((*item)->m_ltype == LayoutItem::TYPE_CELL)
        {
            m_side++;
            m_ss << ",WEST," << m_side << ",I/O,NONE," << (*item)->m_instance << ","<< (*item)->m_cellname << ",,,,,,,,,,,,,,,,\n";
        }
    }
}


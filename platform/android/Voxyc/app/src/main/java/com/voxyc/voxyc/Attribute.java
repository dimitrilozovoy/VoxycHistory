/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

package com.voxyc.voxyc;

public class Attribute {
	public String m_name;
	String m_value;
	boolean m_changed;
	String m_type;
	boolean m_isNull = false;
        AttrType m_attrType = AttrType.SQL;
	
	public Attribute(String name, String value)
	{
		m_name = parseName(name).trim();
		m_type = parseType(name).trim();
		m_value = value;
		m_changed = false;
		if (value == null)
			m_isNull = true;
	}
	
	public Attribute(String name, String value, AttrType type)
	{
            if (type == AttrType.SQL)
            {
		m_name = parseName(name).trim();
		m_type = parseType(name).trim();
            }
            else if (type == AttrType.REGUAR)
            {
                m_name = name;
            }

            m_value = value;
            m_changed = false;
            if (value == null)
                m_isNull = true;
	}

        public Attribute(String name, String value, String type)
	{
		m_name = parseName(name).trim();
		m_type = type.trim();
		m_value = value;
		m_changed = false;
		if (value == null)
			m_isNull = true;
	}
	
	public Attribute(String name, String value, boolean isNull)
	{
		m_name = parseName(name).trim();
		m_type = parseType(name).trim();
		m_value = value;
		m_changed = false;
		m_isNull = isNull;
		if (value == null)
			m_isNull = true;
	}
	
	public Attribute(String name, String value, String type, boolean isNull)
	{
		m_name = parseName(name).trim();
		m_type = type.trim();
		m_value = value;
		m_changed = false;
		m_isNull = isNull;
		if (value == null)
			m_isNull = true;
	}
	
	public String getName()
	{
		return m_name;
	}
	
	public void setName(String name)
	{
		m_name = name.trim();
		m_changed = true;
	}

	public String getValue()
	{
		return m_value;
	}
	
	public void setValue(String value)
	{
		m_value = value;
		if (value == null)
			m_isNull = true;
	}
	
	public void resetChanged()
	{
		m_changed = false;
	}
	
	public String parseType(String s)
	{
		int a;
		
		a = s.indexOf(":");
		
		if (a == -1)
			return "varchar(250)";
		else
			return s.substring(a + 1);
	}

	public String parseName(String s)
	{
		int a;
		
		a = s.indexOf(":");
		
		if (a == -1)
			return s;
		else
		{
			if (a >= 1)
				return s.substring(0, a);
			else
				return "";
		}
	}
	
	public String getType()
	{
		return m_type;
	}
	
	public Boolean isNull()
	{
		if (m_isNull)
			return true;
		else
			return false;	
	}
	
	public void setNull(boolean isNull)
	{
		m_isNull = isNull;
		m_value = "";
	}
	
	public void setType(String type)
	{
		m_type = type.trim();
	}
}

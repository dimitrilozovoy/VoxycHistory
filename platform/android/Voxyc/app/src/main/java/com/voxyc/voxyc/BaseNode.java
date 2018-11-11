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

import java.util.ArrayList;
import java.util.List;
import java.util.Date;
import java.text.DateFormat;

//import org.apache.http.impl.cookie.DateParseException;
//import org.apache.http.impl.cookie.DateUtils;

/**
 *
 * @author dimitri
 */
public class BaseNode
{
//	protected BaseTable m_table = null;
	protected AttrList m_attrs = null;
//	protected BaseTable m_children = null;
//	NodePath m_nodePath = null;
	protected Node m_parent = null;
//	public BaseTree m_tree;
        Boolean m_hasChildren = false;
	
	public BaseNode()
	{
//		m_table = null;
		m_attrs = new AttrList();
	}
		
	public void addAttr(String name, String value)
	{
		m_attrs.addAttr(name, value);
	}

	public void setAttrList(AttrList attrs)
	{		
		m_attrs = attrs;
	}

	public AttrList getAttrs()
	{
		return m_attrs;
	}
	
	public Date getDateValue(String name)
	{
		Date dt;
		
		try
		{
//			dt = DateUtils.parseDate(m_attrs.getValue(name));
                    DateFormat df = DateFormat.getDateInstance();
                    dt = df.parse(name);
		}
		catch (Exception e)
		{
			dt = null;
		}
		
		return dt;
	}
	
	public String getStrValue(String name)
	{
		return m_attrs.getValue(name);
	}
	
	public String get(String name)
        {
            return getStrValueNoNull(name);
        }
        
  	public Double getDbl(String name)
        {
            try
            {
                return Double.valueOf(getStrValueNoNull(name));
            }
            catch (NumberFormatException e)
            {
                return 0.0;
            }
        }

  	public Double getDbl(String name, double errorValue)
        {
            try
            {
                return Double.valueOf(getStrValueNoNull(name));
            }
            catch (NumberFormatException e)
            {
                return errorValue;
            }
        }

	public String getStrValueNN(String name)
        {
            return getStrValueNoNull(name);
        }
        
	public String getStrValueNoNull(String name)
	{
		String s;
		
		s = m_attrs.getValue(name);
		
		if (s != null)
			return s;
		else
			return "";
	}
	
	public int getInt(String name, int errorValue)
	{
	    return getIntValue(name, errorValue);
	}
	
	public int getIntValue(String name, int errorValue)
	{
		int v;
		
		try
		{
			v = Integer.parseInt(m_attrs.getValue(name));
		}
		catch (Exception e)
		{
			v = errorValue;
		}
		
		return v;
	}
	
	public void setValue(String name, String value)
	{
		m_attrs.setValue(name, value);
	}
	
	public void set(String name, String value)
	{
            m_attrs.setValue(name, value);
	}
	
	public void set(String name, int value)
	{
            m_attrs.setValue(name, Integer.toString(value));
	}
	
	public void set(String name, double value)
	{
	    m_attrs.setValue(name, Double.toString(value));
	}

	public void setParent(Node parent)
	{
		m_parent = parent;
	}
	
	public Boolean hasChildren()
	{
/*		if (getStrValueNoNull("haschildren").equals("1"))
		{
			return true;
		}
		else
			return false;*/
            return m_hasChildren;
	}
        
        public void setHasChildren(Boolean hasChildren)
        {
            m_hasChildren = hasChildren;
        }
	
	public String getName()
	{
		return getStrValueNoNull("name");
	}
	
        public Node getParent()
	{
		return m_parent;
	}
	
	public String getAllValuesAsText()
	{
		String text = "";
		
		for (int i = 0; i < m_attrs.size(); i++)
		{
			text += m_attrs.getValue(i);
		}
		
		return text;
	}

        @Override
        public String toString()
        {
            String str = "";
            
            str = getStrValueNoNull("name");
            
//            str = "qqq";

/*            if (m_attrs != null)
                str += m_attrs.toString();
            else
                str += "m_attrs is null;";

            if (m_nodePath != null)
                str += getNodePath().toString();
            else
                str += "m_nodePath is null;";*/

            return str;
        }
        
}

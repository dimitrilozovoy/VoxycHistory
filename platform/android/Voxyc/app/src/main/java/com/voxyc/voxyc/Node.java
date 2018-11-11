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

import java.io.UnsupportedEncodingException;
import java.util.*;


public class Node extends BaseNode {

    String m_tagType = "";
    Boolean m_isContent = false;
    Boolean m_closedTag = false;
    String m_childSortCol = null;
    Boolean m_childNaturalSort = false;

    public Node() {
        setTagType("node");

//        m_table = null;
        m_attrs = new AttrList();
    }

    public Node(String name) {
        setTagType("node");

            
            m_attrs = new AttrList();
            setValue("name", name);
        
    }


    public Boolean isRunnable() {
        return false;
    }


    Boolean isContent() {
        return m_isContent;
    }

    void setIsContent(Boolean isContent) {
        m_isContent = isContent;
    }

    public String getTagType() {
        return m_tagType;
    }

    public void setTagType(String tagType) {
        m_tagType = tagType;
    }

    public Boolean isHTMLText() {
        return false;
    }

    public String getType() {
        return "Node";
    }

    @Override
    public String toString() {
        try {
            return getAttrs().toQueryStr();
        } catch (java.io.UnsupportedEncodingException e) {
            return "";
        }
    }


    public Attribute getAttr(String name) {
        return m_attrs.getAttr(name);
    }

    public void setAttrsFromUrl(String url) {
        AttrList optAttrs = new AttrList();
        optAttrs.addAttrsFromUrl(url);
        setAttrList(optAttrs);
    }


    public boolean exists(String attrName) {
        if (m_attrs != null) {
            return m_attrs.exists(attrName);
        }

        return false;
    }


}

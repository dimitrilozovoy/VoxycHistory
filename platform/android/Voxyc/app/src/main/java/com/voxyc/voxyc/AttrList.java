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
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.net.URLDecoder;

public class AttrList {

    ArrayList<Attribute> m_attrs;


    public AttrList(String urlStr) {
        m_attrs = new ArrayList<Attribute>();

        addAttrsFromUrl(urlStr);
    }

    public AttrList() {
        m_attrs = new ArrayList<Attribute>();
    }

    public void addAttr(String name, String value, String type) {
        if (exists(name)) {
            setValue(name, value);
        } else {
            Attribute newAttr = new Attribute(name, value, type);
            m_attrs.add(newAttr);
        }
    }

    public void addAttr(Attribute newAttr) {
        if (exists(newAttr.getName())) {
            setValue(newAttr.getName(), newAttr.getValue());
        } else {
            m_attrs.add(newAttr);
        }
    }

    public void addAttr(String name, String value) {
        if (exists(name)) {
            setValue(name, value);
        } else {
            Attribute newAttr = new Attribute(name, value);
            m_attrs.add(newAttr);
        }
    }

    public void addAttr(String name, String value, AttrType type) {
        if (exists(name)) {
            setValue(name, value);
        } else {
            Attribute newAttr = new Attribute(name, value, type);
            m_attrs.add(newAttr);
        }
    }

    public void addAttr(String name, String value, boolean isNull) {
        if (exists(name)) {
            setValue(name, value);
        } else {
            Attribute newAttr = new Attribute(name, value, isNull);
            m_attrs.add(newAttr);
        }
    }

    public void addAttr(String name, String value, String type, boolean isNull) {
        if (exists(name)) {
            setValue(name, value);
        } else {
            Attribute newAttr = new Attribute(name, value, type, isNull);
            m_attrs.add(newAttr);
        }
    }

    public void addAttrs(AttrList attrs) {
        for (int i = 0; i < attrs.getList().size(); i++) {
            if (exists(attrs.getList().get(i).getName())) {
                if (!attrs.getList().get(i).isNull()) {
                    Attribute mya = getAttr(i);
//					mya.setValue(attrs.getList().get(i).m_value);
//					mya.setType(attrs.getList().get(i).m_type);
                    setValue(attrs.getList().get(i).m_name, attrs.getList().get(i).m_value);
                }
            } else {
                addAttr(attrs.getList().get(i).getName(), attrs.getList().get(i).getValue(), attrs.getList().get(i).getType());
            }
        }
    }

    public String getValue(String name) {
        for (int i = 0; i < m_attrs.size(); i++) {
            if (name.trim().equals(m_attrs.get(i).getName().trim())) {
                return m_attrs.get(i).getValue();
            }
        }

        return null;
    }

    public String getValue(int i) {
        return m_attrs.get(i).getValue();
    }

    public String getName(int i) {
        return m_attrs.get(i).getName();
    }

    public void setValue(String name, String value) {
        Boolean found = false;

        for (int i = 0; i < m_attrs.size(); i++) {
            if (name.trim().equals(m_attrs.get(i).getName().trim())) {
                m_attrs.get(i).setValue(value);
                found = true;
            }
        }

        if (!found) {
            addAttr(name, value);
        }
    }

    public void setValue(int i, String value) {
        m_attrs.get(i).setValue(value);
    }

    public boolean exists(String name) {
        for (int i = 0; i < m_attrs.size(); i++) {
            if (name.trim().equals(m_attrs.get(i).getName().trim())) {
                return true;
            }
        }

        return false;
    }

    public ArrayList<Attribute> getList() {
        return m_attrs;
    }

    public String toQueryStr() throws UnsupportedEncodingException {
        String qs = "";

        for (int i = 0; i < m_attrs.size(); i++) {
            if (qs != "") {
                qs += "&";
            }

            qs += URLEncoder.encode(m_attrs.get(i).getName(), "UTF-8") + "=" + URLEncoder.encode(m_attrs.get(i).getValue(), "UTF-8");
        }

        return qs;
    }


    public int size() {
        return m_attrs.size();
    }

    public int count() {
        return m_attrs.size();
    }

    public String getType(int i) {
        return m_attrs.get(i).m_type;
    }

    public Attribute getAttr(int i) {
        return m_attrs.get(i);
    }

    public Attribute getAttr(String name) {
        Boolean found = false;

        for (int i = 0; i < m_attrs.size(); i++) {
            if (name.trim().equals(m_attrs.get(i).getName().trim())) {
                return m_attrs.get(i);
            }
        }

        Attribute a = new Attribute(name, "");
        m_attrs.add(a);
        return a;
    }

    public void addAttrsFromUrl(String url) {
        int posQMark = -1;
        int pos = 0;
        char c;
        String name = "";
        String value = "";
        Boolean readingName;
        Boolean readingValue;

        posQMark = url.indexOf('?');

        if (posQMark == -1) {
            pos = 0;
        } else {
            pos = posQMark + 1;
        }

        if (pos != -1) {
            readingName = true;
            readingValue = false;

            while (pos < url.length()) {
                c = url.charAt(pos);

                switch (c) {
                    case '&':
                        if (!name.equals("")) {
                            try {
                                Attribute attr = new Attribute(name, URLDecoder.decode(value, "UTF-8"));
//                            Attribute attr = new Attribute(name, value);
                                m_attrs.add(attr);
                            } catch (Exception e) {
		//						Lozoware.exception(e);
                            }
                        }

                        readingName = true;
                        name = "";
                        readingValue = false;
                        value = "";
                        break;
                    case '=':
                        readingValue = true;
                        readingName = false;
                        break;
                    default:
                        if (readingValue) {
                            value += c;
                        }
                        if (readingName) {
                            name += c;
                        }
                        break;
                }

                pos++;
            }

            // Add last parameter
            if (!name.equals("")) {
//				Attribute attr = new Attribute(name, value);
                try {
                    Attribute attr = new Attribute(name, URLDecoder.decode(value, "UTF-8"));
                    m_attrs.add(attr);
                } catch (Exception e) {
	//				Lozoware.exception(e);
                }
            }
        }
    }

    @Override
    public String toString() {
        String str = "";

        for (int i = 0; i < m_attrs.size(); i++) {
//                if (str.length() > 0)
//                    str += ";";

            str += m_attrs.get(i).getName() + "=" + m_attrs.get(i).getValue() + ";";
        }

        return str;
    }

    public void deleteAttr(String name) {
        Boolean found = false;

        for (int i = 0; i < m_attrs.size(); i++) {
            if (name.trim().equals(m_attrs.get(i).getName().trim())) {
                m_attrs.remove(i);
                return;
            }
        }
    }
}

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

import android.app.*;

import android.app.AlertDialog;
import android.content.*;
import android.widget.*;
import java.io.*;
import android.view.ViewGroup.*;
import java.util.*;
import android.view.*;
import android.graphics.*;

public class DDLUtils
{
	interface ActionDone
	{
		public String input = "";
		public String folder = "";
		public String filename = "";
		public String result = "";
		
		void onActionDone();
	}
	
	public static Activity mainActivity;
	
    // List menu vars
    static ArrayList<View> m_lmViews = null;
    static ArrayList<RespondToNode> m_lmCallbacks = null;
	static ArrayList<RespondToNode> m_lmLongCallbacks = null;
	static ArrayList<String> m_lmCaptions = null;
	static int m_numListMenuOptions = 0;
	static int m_mediumTextSize = 20;
	
	static int m_smallTextSize = 20;
	
	
	public static int getContentHeight()
	{
		return getScreenHeight();
	}
	
	public static int getContentWidth()
	{
		return getScreenWidth();
	}
	
	public static int getScreenHeight()
	{
		return mainActivity.getWindowManager().getDefaultDisplay().getHeight();
	}

	public static int getScreenWidth()
	{
		return mainActivity.getWindowManager().getDefaultDisplay().getWidth();
	}
	
	public static void consoleInUIThread(String str)
	{
	/*	RespondToNodeRunnable r = new RespondToNodeRunnable() {
            public void respondToNode() {
                console(m_input.get("text"));
            }
        };

        r.m_input = new Node();
        r.m_input.set("text", str);

        runInUiThread(r);

*/
		
	}
	
	public static void toast(Activity a, String str)
	{
		Context context = a.getApplicationContext();
		CharSequence text = str;
		int duration = Toast.LENGTH_SHORT;

		Toast toast = Toast.makeText(context, text, duration);
		toast.show();
	}
	
	public static int getScreenDiagonalSize()
	{
		return 768;
	}
		
    public static void showFileSelector(final String title, String filename, final RespondToNode whenDone, final boolean newFileOption) {

        File folder = null;

        if (filename == "")
        {
            folder = new File("/");
        }
        else
        {
            folder = new File(filename);
        }

        if (folder == null)
            return;

        if (!folder.isDirectory()) {
            folder = folder.getParentFile();
        }

        if (folder != null) {

            clearListMenu();

            // Go up option, if folder has parent
            if (folder.getParentFile() != null) {

                RespondToNode r = new RespondToNode() {
                    @Override
                    public void respondToNode() {
                        showFileSelector(title, m_input.get("filename"), whenDone, newFileOption);
                    }
                };

                r.m_input = new Node();
                r.m_input.set("filename", folder.getParentFile().getPath());

                addListMenuOption("[..]", "", r, null);
            }

            if (newFileOption) {
                // New file option
                RespondToNode r = new RespondToNode() {
                    @Override
                    public void respondToNode() {
                        RespondToNode r = new RespondToNode() {
                            @Override
                            public void respondToNode() {
                                whenDone.m_input = new Node();
                                whenDone.m_input.set("filename", m_input.get("folder") + "/" + m_input.get("result"));
                                whenDone.respondToNode();
                            }
                        };

                        r.m_input = new Node();
                        r.m_input.set("folder", m_input.get("folder"));
                        showGetStringDialog("New file in " + m_input.get("folder"), "", r);
                    }
                };

                r.m_input = new Node();
                r.m_input.set("folder", folder.getPath());

                addListMenuOption("(new file...)", "", r, null);
            }

            // List the files and folders
            File[] listOfFiles = folder.listFiles();

            if (listOfFiles != null) {
                for (final File file : listOfFiles) {
                    if (file != null) {

                        // If this is a folder
                        if (file.isDirectory()) {
                            RespondToNode r2 = new RespondToNode() {
                                @Override
                                public void respondToNode() {
                                    showFileSelector(title, m_input.get("folder"), whenDone, newFileOption);
                                }
                            };

                            r2.m_input = new Node();
                            r2.m_input.set("folder", file.getPath());

                            addListMenuOption("[" + file.getName() + "]", "", r2, null);
                        }

                        // If this is a file
                        if (file.isFile()) {
                            RespondToNode r3 = new RespondToNode() {
                                @Override
                                public void respondToNode() {
                                    whenDone.m_input = new Node();
                                    whenDone.m_input.set("filename", m_input.get("filename"));
                                    whenDone.respondToNode();
                                }
                            };

                            r3.m_input = new Node();
                            r3.m_input.set("filename", file.getPath());

                            addListMenuOption(file.getName(), "", r3, null);
                        }
                    }
                }
            }

            // Show list menu
            showListMenuInDialog(title, "");
        }
    }

    
    public static void showFileSelectorWithPrompt(final String title, final String filename, final RespondToNode whenDone, final boolean newFileOption)
    {

        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
        final EditText input = new EditText(mainActivity.getApplication());
        input.setText(filename);
        input.setTextColor(Color.BLUE);
        alert.setView(input);
        alert.setTitle(title);

        // OK button

        LozowareOCListener onOK = new LozowareOCListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                if (m_rtn.m_input == null) {
                    m_rtn.m_input = new Node();
                }

                m_rtn.m_input.set("filename", input.getText().toString().trim());
                m_rtn.respondToNode();
            }
        };

        onOK.m_rtn = whenDone;

        alert.setPositiveButton("OK", onOK);

        // Picker

        LozowareOCListener onPicker = new LozowareOCListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
//                if (m_rtn.m_input == null) {
//                    m_rtn.m_input = new Node();
//                }

                showFileSelector(title, filename, whenDone, newFileOption);

//                m_rtn.m_input.set("result", input.getText().toString().trim());
//                m_rtn.respondToNode();
            }
        };

//        onPicker.m_rtn = whenDone;

        alert.setNeutralButton("Picker", onPicker);

        // Cancel button

        alert.setNegativeButton("Cancel",
			new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton) {
					dialog.cancel();
				}
			});

        alert.show();

    }
	
///////////////////////////////////////////////////////////////////
//
//  List menu
//
///////////////////////////////////////////////////////////////////

	class ListMenuViewAdapter extends BaseAdapter {

		public ListMenuViewAdapter() {
		}

		//---returns the number of images---
		public int getCount() {
			return getListMenuViews().size();
		}

		//---returns the ID of an item--- 
		public Object getItem(int position) {
			return position;
		}

		public long getItemId(int position) {
			return position;
		}

		public View getView(int position, View convertView, ViewGroup parent) {
			return getListMenuViews().get(position);
		}
	}
	
    public static void clearListMenu() {
//        if (m_lmView != null) {
 //           m_lmView.removeViews(0, m_lmView.getChildCount());
 //       }
        if (m_lmViews != null) {
            m_lmViews.clear();
        }
        if (m_lmCallbacks != null) {
            m_lmCallbacks.clear();
        }
        if (m_lmLongCallbacks != null) {
            m_lmLongCallbacks.clear();
        }
        if (m_lmCaptions != null) {
            m_lmCaptions.clear();
        }

        m_numListMenuOptions = 0;
    }


    public static void addListMenuOption(String caption, String description, final RespondToNode al, final RespondToNode longPress) {
        addListMenuOption(caption, description, al, null, "");
    }


    public static void addListMenuOption(String caption, String description, final RespondToNode al, final RespondToNode longPress, String options) {
        addListMenuOption(caption, description, al, longPress, options, null);
    }

    
    public static void addListMenuOption(String caption, String description, final RespondToNode al, final RespondToNode longPress, String options, Attribute attr) {
        
        Boolean dontCallAl = false;

        if (m_lmViews == null) {
            m_lmViews = new ArrayList<View>();
        }

        if (m_lmCallbacks == null) {
            m_lmCallbacks = new ArrayList<RespondToNode>();
        }

        if (m_lmLongCallbacks == null) {
            m_lmLongCallbacks = new ArrayList<RespondToNode>();
        }

        if (m_lmCaptions == null) {
            m_lmCaptions = new ArrayList<String>();
        }

/*        if (m_lmView == null) {
            ListMenuViewAdapter adapter = new ListMenuViewAdapter();
            m_lmView = new ListView(m_currentActivity.getApplication());
            m_lmView.setAdapter(adapter);
            m_lmView.setOnItemClickListener(
				new AdapterView.OnItemClickListener() {
					public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
						if (m_lmCallbacks.get(position) != null) {
							m_lmCallbacks.get(position).respondToNode();
						}
					}
				});
            m_lmView.setOnItemLongClickListener(
				new AdapterView.OnItemLongClickListener() {
					public boolean onItemLongClick(AdapterView<?> arg0, View arg1, int position, long arg4) {

						LozowareUI ui = Lozoware.getUI();



						if (m_lmLongCallbacks.get(position) != null) {
							m_lmLongCallbacks.get(position).respondToNode();
						} else {
						}

						return true;
					}
				});
        }*/

        // Text view - the big text inside the list view item
        TextView tv = new TextView(mainActivity);
        tv.setText(DDLUtils.cutOffStr(caption, 30));
        LinearLayout.LayoutParams tlp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT, 1.0f);
        tv.setLayoutParams(tlp);

        // Description view - the tiny text under the main text that gives details about the list option
        TextView dv = new TextView(mainActivity);
        dv.setText(DDLUtils.cutOffStr(description, 80));
        LinearLayout.LayoutParams dlp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT, 1.0f);
        dv.setLayoutParams(dlp);

        // Create vertical layout
        LinearLayout.LayoutParams vlp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT, 1.0f);
        LinearLayout vl = new LinearLayout(mainActivity);
        vl.setLayoutParams(vlp);
        vl.setOrientation(LinearLayout.VERTICAL);

        // Create horizontal layout
        AbsListView.LayoutParams hlp = new AbsListView.LayoutParams(getScreenWidth() * 100 / 90, AbsListView.LayoutParams.WRAP_CONTENT);
        LinearLayout hl = new LinearLayout(mainActivity);
        hl.setLayoutParams(hlp);
        hl.setOrientation(LinearLayout.HORIZONTAL);

        vl.addView(tv);
        vl.addView(dv);
        hl.addView(vl);

        // In case that this will go into a dialog
        m_lmCaptions.add(caption);
        m_numListMenuOptions++;

        // Assign options if necessary
        if (options != null) {
            Node optNode = new Node();
            optNode.setAttrsFromUrl(options);

            if (optNode.get("style").equals("test")) {
                tv.setTextColor(Color.rgb(255, 128, 0));
                tv.setTextSize(m_mediumTextSize);

                dv.setTextColor(Color.rgb(160, 160, 160));
                dv.setTextSize(m_smallTextSize);
            }

            if (optNode.get("checkbox").equals("1")) {
            }
        }

        m_lmViews.add(hl);

        if (dontCallAl) {
            m_lmCallbacks.add(null);
            m_lmLongCallbacks.add(null);
        } else {
            m_lmCallbacks.add(al);
            m_lmLongCallbacks.add(longPress);
        }
    }

    
    public void addListMenuOptions() {
    }
	

    //
    // LISTMENU IN A DIALOG
    //
    public static void showListMenuInDialog(String title, String options) {
        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
        final EditText input = new EditText(mainActivity.getApplication());

        alert.setTitle(title);

        String[] captions = new String[m_numListMenuOptions];
        for (int i = 0; i < m_numListMenuOptions; i++) {
            captions[i] = m_lmCaptions.get(i);
        }

        alert.setItems(captions, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					if (m_lmCallbacks.get(which) != null) {
						m_lmCallbacks.get(which).respondToNode();
					}

				}
			});

        // Assign options if necessary
        if (options != null) {
            Node optNode = new Node();
            optNode.setAttrsFromUrl(options);

            if (optNode.get("style").equals("test")) {
            }
        }

        alert.show();
    }

    
    public void showListMenu() {
  /*      if (m_lmView == null) {
            return;
        }

        if (m_lmViews == null) {
            return;
        }

        m_viewingMap = false;
        deflateMap();

        clearContentView();
        setContentView(m_lmView);*/
    }

    public static ArrayList<View> getListMenuViews() {
        return m_lmViews;
    }

	
///////////////////////////////////////////////////////////////////
//
//  Misc
//
///////////////////////////////////////////////////////////////////

    
    public static String showGetStringDialog(String title, String oldValue, RespondToNode node) {

        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
        final EditText input = new EditText(mainActivity.getApplication());
        input.setText(oldValue);
        alert.setView(input);
        alert.setTitle(title);

        LozowareOCListener pl = new LozowareOCListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                if (m_rtn.m_input == null) {
                    m_rtn.m_input = new Node();
                }

                m_rtn.m_input.set("result", input.getText().toString().trim());
                m_rtn.respondToNode();
            }
        };

        pl.m_rtn = node;

        alert.setPositiveButton("OK", pl);

        alert.setNegativeButton("Cancel",
			new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton) {
					dialog.cancel();
				}
			});

        alert.show();

        return "";
	}
    
    public static void showOKCancelDialog(String title, String message, RespondToNode node) {
        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
        alert.setTitle(title);
        alert.setMessage(message);

        LozowareOCListener pl = new LozowareOCListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                m_rtn.respondToNode();
            }
        };

        pl.m_rtn = node;

        alert.setPositiveButton("OK", pl);

        alert.setNegativeButton("Cancel",
			new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton) {
					dialog.cancel();
				}
			});

        alert.show();

        return;
    }

    
    public static void showOKDialog(String title, String message, RespondToNode node) {
        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
        alert.setTitle(title);
        alert.setMessage(message);

        LozowareOCListener pl = new LozowareOCListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                m_rtn.respondToNode();
            }
        };

        pl.m_rtn = node;

        alert.setPositiveButton("OK", pl);

        alert.show();

        return;
    }
	
    public static void showText(String text) {
        final AlertDialog.Builder alert = new AlertDialog.Builder(mainActivity);
//        alert.setTitle(title);
        alert.setMessage(text);

        alert.setPositiveButton("OK", null);

        alert.show();

        return;
    }
	
		
	public static String cutOffExt(String fname) {
        int i;
//		 Roamer.showMsg("cutoffext: " + fname);

        i = fname.lastIndexOf(".");

        if (i != -1 && i >= 1) {
            return fname.substring(0, fname.lastIndexOf("."));
        } else {
            return fname;
        }
    }
	

    public static String cutOffStr(String s, int len) {
        if (s.length() > len) {
            return s.substring(0, len) + "...";
        } else {
            return s;
        }
    }
	
}

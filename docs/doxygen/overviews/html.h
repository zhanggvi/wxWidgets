/////////////////////////////////////////////////////////////////////////////
// Name:        html
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*!
 
 @page html_overview wxHTML overview
 
 The wxHTML library provides classes for parsing and displaying HTML.
 It is not intended to be a high-end HTML browser. If you are looking for
 something like that try #http://www.mozilla.org.
 wxHTML can be used as a generic rich text viewer - for example to display 
 a nice About Box (like those of GNOME apps) or to display the result of
 database searching. There is a #wxFileSystem 
 class which allows you to use your own virtual file systems.
 wxHtmlWindow supports tag handlers. This means that you can easily
 extend wxHtml library with new, unsupported tags. Not only that,
 you can even use your own application-specific tags!
 See @c src/html/m_*.cpp files for details.
 There is a generic wxHtmlParser class,
 independent of wxHtmlWindow.
 @ref htmlquickstart_overview
 @ref printing_overview
 @ref helpformat_overview
 @ref filters_overview
 @ref cells_overview
 @ref handlers_overview
 @ref htmltagssupported_overview
 
 
 @section wxhtmlquickstart wxHTML quick start
 
 @b Displaying HTML
 First of all, you must include wx/wxhtml.h.
 Class #wxHtmlWindow (derived from wxScrolledWindow)
 is used to display HTML documents.
 It has two important methods: #LoadPage 
 and #SetPage.
 LoadPage loads and displays HTML file while SetPage displays directly the
 passed @b string. See the example:
 
 @code
 mywin - LoadPage("test.htm");
     mywin - SetPage("htmlbody"
                      "h1Error/h1"
                      "Some error occurred :-H)"
                      "/body/hmtl");
 @endcode
 
 @b Displaying Help
 See #wxHtmlHelpController.
 @b Setting up wxHtmlWindow
 Because wxHtmlWindow is derived from wxScrolledWindow and not from
 wxFrame, it doesn't have visible frame. But the user usually wants to see
 the title of HTML page displayed somewhere and the frame's titlebar is 
 the ideal place for it.
 wxHtmlWindow provides 2 methods in order to handle this: 
 #SetRelatedFrame and 
 #SetRelatedStatusBar. 
 See the example:
 
 @code
 html = new wxHtmlWindow(this);
     html - SetRelatedFrame(this, "HTML : %%s");
     html - SetRelatedStatusBar(0);
 @endcode
 
 The first command associates the HTML object with its parent frame
 (this points to wxFrame object there) and sets the format of the title.
 Page title "Hello, world!" will be displayed as "HTML : Hello, world!"
 in this example.
 The second command sets which frame's status bar should be used to display
 browser's messages (such as "Loading..." or "Done" or hypertext links).
 @b Customizing wxHtmlWindow
 You can customize wxHtmlWindow by setting font size, font face and
 borders (space between border of window and displayed HTML). Related functions:
 
 
  #SetFonts
  #SetBorders
  #ReadCustomization
  #WriteCustomization
 
 
 The last two functions are used to store user customization info wxConfig stuff
 (for example in the registry under Windows, or in a dotfile under Unix).
 
 @section printing HTML Printing
 
 The wxHTML library provides printing facilities with several levels of complexity. 
 The easiest way to print an HTML document is to use 
 @ref htmleasyprinting_overview. It lets you print HTML documents with only one
 command and you don't have to worry about deriving from the wxPrintout class at all. It is only a simple wrapper around the 
 #wxHtmlPrintout, normal wxWidgets printout class.
 And finally there is the low level class #wxHtmlDCRenderer which you can use to
 render HTML into a rectangular area on any DC. It supports rendering into multiple rectangles with the same
 width. (The most common use of this is placing one rectangle on each page or printing into two columns.)
 
 @section helpformat Help Files Format
 
 wxHTML library uses a reduced version of MS HTML Workshop format.
 Tex2RTF can produce these files when generating HTML, if you set @b htmlWorkshopFiles to @b @true in
 your tex2rtf.ini file.
 (See #wxHtmlHelpController for help controller description.)
 A @b book consists of three files: header file, contents file and index file.
 You can make a regular zip archive of these files, plus the HTML and any image files,
 for wxHTML (or helpview) to read; and the .zip file can optionally be renamed to .htb.
 @b Header file (.hhp)
 Header file must contain these lines (and may contain additional lines which are ignored) :
 
 @code
 Contents file=filename.hhc
 Index file=filename.hhk
 Title=title of your book
 Default topic=default page to be displayed.htm
 @endcode
 
 All filenames (including the Default topic) are relative to the 
 location of .hhp file.
 @b Localization note: In addition, .hhp file may contain line
 
 @code
 Charset=rfc_charset
 @endcode
 
 which specifies what charset (e.g. "iso8859_1") was used in contents
 and index files. Please note that this line is incompatible with
 MS HTML Help Workshop and it would either silently remove it or complain
 with some error. See also 
 @ref nonenglish_overview.
 @b Contents file (.hhc)
 Contents file has HTML syntax and it can be parsed by regular HTML parser. It contains exactly one list
 (@c ul....@c /ul statement):
 
 @code
 ul
 
   li object type="text/sitemap"
            param name="Name" value="@topic name@"
            param name="ID" value=@numeric_id@
            param name="Local" value="@filename.htm@"
        /object
   li object type="text/sitemap"
            param name="Name" value="@topic name@"
            param name="ID" value=@numeric_id@
            param name="Local" value="@filename.htm@"
        /object
   ...    
 
 /ul
 @endcode
 
 You can modify value attributes of param tags. @e topic name is name of chapter/topic as is displayed in
 contents, @e filename.htm is HTML page name (relative to .hhp file) and @e numeric_id is optional 
 - it is used only when you use wxHtmlHelpController::Display(int)
 Items in the list may be nested - one @c li statement may contain a @c ul sub-statement:
 
 @code
 ul
 
   li object type="text/sitemap"
            param name="Name" value="Top node"
            param name="Local" value="top.htm"
        /object
        ul
          li object type="text/sitemap"
               param name="Name" value="subnode in topnode"
               param name="Local" value="subnode1.htm"
               /object
       ...
        /ul
        
   li object type="text/sitemap"
            param name="Name" value="Another Top"
            param name="Local" value="top2.htm"
        /object
   ...    
 
 /ul
 @endcode
 
 @b Index file (.hhk)
 Index files have same format as contents file except that ID params are ignored and sublists are @b not 
 allowed.
 
 @section filters Input Filters
 
 The wxHTML library provides a mechanism for reading and displaying
 files of many different file formats. 
 wxHtmlWindow::LoadPage can load not
 only HTML files but any known file. To make a file type known to wxHtmlWindow
 you must create a #wxHtmlFilter filter and
 register it using wxHtmlWindow::AddFilter.
 
 @section cells Cells and Containers
 
 This article describes mechanism used by 
 #wxHtmlWinParser and 
 #wxHtmlWindow to parse and display HTML documents.
 @b Cells
 You can divide any text (or HTML) into small fragments. Let's call these
 fragments @b cells. Cell is for example one word, horizontal line, image
 or any other part of document. Each cell has width and height (except special
 "magic" cells with zero dimensions - e.g. colour changers or font changers).
 See #wxHtmlCell.
 @b Containers
 Container is kind of cell that may contain sub-cells. Its size depends
 on number and sizes of its sub-cells (and also depends on width of window). 
 See #wxHtmlContainerCell, 
 wxHtmlCell::Layout.
 This image shows the cells and containers:
 
 @b Using Containers in Tag Handler
 #wxHtmlWinParser provides a user-friendly way
 of managing containers. It is based on the idea of opening and closing containers.
 Use #OpenContainer to open new
 a container @e within an already opened container. This new container is a 
 @e sub-container of the old one. (If you want to create a new container with
 the same depth level you can call @c CloseContainer(); OpenContainer();.)
 Use #CloseContainer to close the 
 container. This doesn't create a new container with same depth level but
 it returns "control" to the parent container.
 See explanation:
 
 There clearly must be same number of calls to OpenContainer as to
 CloseContainer.
 @b Example
 This code creates a new paragraph (container at same depth level)
 with "Hello, world!":
 
 @code
 m_WParser - CloseContainer();
 c = m_WParser - OpenContainer();
 
 m_WParser - AddText("Hello, ");
 m_WParser - AddText("world!");
 
 m_WParser - CloseContainer();
 m_WParser - OpenContainer();
 @endcode
 
 and here is image of the situation:
 
 You can see that there was an opened container before the code was executed.
 We closed it, created our own container, then closed our container and opened
 new container. The result was that we had @e same depth level after
 executing. This is general rule that should be followed by tag handlers:
 leave depth level of containers unmodified (in other words, number of
 OpenContainer and CloseContainer calls should be same within #HandleTag's body).
 Notice that it would be usually better to use 
 wxHtmlContainerCell::InsertCell instead
 of adding text to the parser directly.
 
 @section handlers Tag Handlers
 
 The wxHTML library provides architecture of pluggable @e tag handlers.
 Tag handler is class that understands particular HTML tag (or tags) and is
 able to interpret it.
 #wxHtmlWinParser has static table of @b modules.
 Each module contains one or more tag handlers. Each time a new wxHtmlWinParser
 object is constructed all modules are scanned and handlers are added
 to wxHtmlParser's list of available handlers (note: wxHtmlParser's list
 is non-static).
 @b How it works
 Common tag handler's #HandleTag method
 works in four steps:
 
 
  Save state of parent parser into local variables
  Change parser state according to tag's params
  Parse text between the tag and paired ending tag (if present)
  Restore original parser state
 
 
 See #wxHtmlWinParser for methods for modifying
 parser's state. In general you can do things like opening/closing containers,
 changing colors, fonts etc.
 @b Providing own tag handlers
 You should create new .cpp file and place following lines into it: 
 
 @code
 #include mod_templ.h
 #include forcelink.h
 FORCE_LINK_ME(yourmodulefilenamewithoutcpp)
 @endcode
 
 Then you must define handlers and one module.
 @b Tag handlers
 The handler is derived from #wxHtmlWinTagHandler
 (or directly from #wxHtmlTagHandler)
 You can use set of macros to define the handler (see src/html/m_*.cpp files
 for details). Handler definition must start with @b TAG_HANDLER_BEGIN macro
 and end with @b TAG_HANDLER_END macro. I strongly recommend to have a look
 at @e include/wxhtml/mod_templ.h file. Otherwise you won't understand
 the structure of macros. See macros reference:
 @b TAG_HANDLER_BEGIN(@e name, @e tags)
 Starts handler definition. @e name is handler identifier (in fact
 part of class name), @e tags is string containing list of tags
 supported by this handler (in uppercase). This macro derives new class from
 wxHtmlWinTagHandler and implements it is 
 #GetSupportedTags method.
 Example: TAG_HANDLER_BEGIN(FONTS, "B,I,U,T")
 @b TAG_HANDLER_VARS
 This macro starts block of variables definitions. (Variables are identical
 to class attributes.) Example:
 
 @code
 TAG_HANDLER_BEGIN(VARS_ONLY, "CRAZYTAG")
     TAG_HANDLER_VARS
         int my_int_var;
 	wxString something_else;
 TAG_HANDLER_END(VARS_ONLY)
 @endcode
 
 This macro is used only in rare cases.
 @b TAG_HANDLER_CONSTR(@e name)
 This macro supplies object constructor. @e name is same name as the one
 from TAG_HANDLER_BEGIN macro. Body of constructor follow after
 this macro (you must use  and  ). Example:
 
 @code
 TAG_HANDLER_BEGIN(VARS2, "CRAZYTAG")
     TAG_HANDLER_VARS
         int my_int_var;
     TAG_HANDLER_CONSTR(vars2)
         { // !!!!!!
 	    my_int_var = 666;
 	} // !!!!!!
 TAG_HANDLER_END(VARS2)
 @endcode
 
 Never used in wxHTML :-)
 @b TAG_HANDLER_PROC(@e varib)
 This is very important macro. It defines #HandleTag
 method. @e varib is name of parameter passed to the method, usually
 @e tag. Body of method follows after this macro.
 Note than you must use  and  ! Example:
 
 @code
 TAG_HANDLER_BEGIN(TITLE, "TITLE")
     TAG_HANDLER_PROC(tag)
         {
 	    printf("TITLE found...\n");
 	}
 TAG_HANDLER_END(TITLE)
 @endcode
 
 @b TAG_HANDLER_END(@e name)
 Ends definition of tag handler @e name. 
 @b Tags Modules
 You can use set of 3 macros TAGS_MODULE_BEGIN, TAGS_MODULE_ADD and 
 TAGS_MODULE_END to inherit new module from
 #wxHtmlTagsModule and to create instance of it.
 See macros reference:
 @b TAGS_MODULE_BEGIN(@e modname)
 Begins module definition. @e modname is part of class name and must
 be unique.
 @b TAGS_MODULE_ADD(@e name)
 Adds the handler to this module. @e name is the identifier from
 TAG_HANDLER_BEGIN.
 @b TAGS_MODULE_END(@e modname)
 Ends the definition of module.
 @b Example:
 
 @code
 TAGS_MODULE_BEGIN(Examples)
     TAGS_MODULE_ADD(VARS_ONLY)
     TAGS_MODULE_ADD(VARS2)
     TAGS_MODULE_ADD(TITLE)
 TAGS_MODULE_END(Examples)
 @endcode
 
 
 @section htmltagssupported Tags supported by wxHTML
 
 wxHTML is not full implementation of HTML standard. Instead, it supports most common tags so that it 
 is possible to display @e simple HTML documents with it. (For example it works fine with pages created
 in Netscape Composer or generated by tex2rtf).
 Following tables list all tags known to wxHTML, together with supported parameters.
 A tag has general form of @c tagname param_1 param_2 ... param_n where param_i is
 either @c paramname="paramvalue" or @c paramname=paramvalue - these two are equivalent. Unless stated 
 otherwise, wxHTML is case-insensitive.
 @b Table of common parameter values
 We will use these substitutions in tags descriptions:
 
 @code
 [alignment]     CENTER
                 LEFT
                 RIGHT
                 JUSTIFY
 
 [v_alignment]   TOP
                 BOTTOM
                 CENTER
                 
 [color]         HTML 4.0-compliant colour specification
 
 [fontsize]      -2
                 -1
                 +0
                 +1
                 +2
                 +3
                 +4
                  1
                  2
                  3
                  4
                  5
                  6
                  7
 
 [pixels]        integer value that represents dimension in pixels
 
 [percent]       i% 
                 where i is integer
 
 [url]           an URL 
 
 [string]        text string
 
 [coords]        c(1),c(2),c(3),...,c(n)
                 where c(i) is integer
 @endcode
 
 
 @b List of supported tags
 
 @code
 A               NAME=[string]
                 HREF=[url]
                 TARGET=[target window spec]
 ADDRESS
 AREA            SHAPE=POLY
                 SHAPE=CIRCLE
                 SHAPE=RECT
                 COORDS=[coords]
                 HREF=[url]
 B
 BIG
 BLOCKQUOTE
 BODY            TEXT=[color]
                 LINK=[color]
                 BGCOLOR=[color]
 BR              ALIGN=[alignment]
 CENTER
 CITE
 CODE
 DD
 DIV             ALIGN=[alignment]
 DL
 DT
 EM
 FONT            COLOR=[color]
                 SIZE=[fontsize]
                 FACE=[comma-separated list of facenames]
 HR              ALIGN=[alignment]
                 SIZE=[pixels]
                 WIDTH=[percent|pixels]
                 NOSHADE
 H1
 H2
 H3
 H4
 H5
 H6
 I
 IMG             SRC=[url]
                 WIDTH=[pixels]
                 HEIGHT=[pixels]
                 ALIGN=TEXTTOP
                 ALIGN=CENTER
                 ALIGN=ABSCENTER
                 ALIGN=BOTTOM
                 USEMAP=[url]
 KBD
 LI
 MAP             NAME=[string]
 META            HTTP-EQUIV="Content-Type"
                 CONTENT=[string]
 OL
 P               ALIGN=[alignment]
 PRE
 SAMP
 SMALL
 STRIKE
 STRONG
 SUB
 SUP
 TABLE           ALIGN=[alignment]
                 WIDTH=[percent|pixels]
                 BORDER=[pixels]
                 VALIGN=[v_alignment]
                 BGCOLOR=[color]
                 CELLSPACING=[pixels]
                 CELLPADDING=[pixels]
 TD              ALIGN=[alignment]
                 VALIGN=[v_alignment]
                 BGCOLOR=[color]
                 WIDTH=[percent|pixels]
                 COLSPAN=[pixels]
                 ROWSPAN=[pixels]
                 NOWRAP
 TH              ALIGN=[alignment]
                 VALIGN=[v_alignment]
                 BGCOLOR=[color]
                 WIDTH=[percent|pixels]
                 COLSPAN=[pixels]
                 ROWSPAN=[pixels]
 TITLE
 TR              ALIGN=[alignment]
                 VALIGN=[v_alignment]
                 BGCOLOR=[color]
 TT
 U
 UL
 @endcode
 
 */
 
 

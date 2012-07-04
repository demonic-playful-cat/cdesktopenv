/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these librararies and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/*** DTB_USER_CODE_START vvv Add file header below vvv ***/
/*
 *      $XConsortium: appfw_stubs.c /main/5 1996/08/07 19:45:56 mustafa $
 *
 * @(#)appfw_stubs.c	1.35 01 May 1995 cde_app_builder/src/ab
 *
 *      RESTRICTED CONFIDENTIAL INFORMATION:
 *
 *      The information in this document is subject to special
 *      restrictions in a confidential disclosure agreement between
 *      HP, IBM, Sun, USL, SCO and Univel.  Do not distribute this
 *      document outside HP, IBM, Sun, USL, SCO, or Univel without
 *      Sun's specific written approval.  This document and all copies
 *      and derivative works thereof must be returned or destroyed at
 *      Sun's request.
 *
 *      Copyright 1993 Sun Microsystems, Inc.  All rights reserved.
 *
 */

/*** DTB_USER_CODE_END   ^^^ Add file header above ^^^ ***/

/*
 * File: appfw_stubs.c
 * Contains: Module callbacks and connection functions
 *
 * This file was generated by dtcodegen, from module appfw
 *
 * Any text may be added between the DTB_USER_CODE_START and
 * DTB_USER_CODE_END comments (even non-C code). Descriptive comments
 * are provided only as an aid.
 *
 *  ** EDIT ONLY WITHIN SECTIONS MARKED WITH DTB_USER_CODE COMMENTS.  **
 *  ** ALL OTHER MODIFICATIONS WILL BE OVERWRITTEN. DO NOT MODIFY OR  **
 *  ** DELETE THE GENERATED COMMENTS!                                 **
 */

#include <stdio.h>
#include <Xm/Xm.h>
#include "dtb_utils.h"
#include "dtbuilder.h"
#include "appfw_ui.h"

/*
 * Header files for cross-module connections
 */
#include "palette_ui.h"


/**************************************************************************
 *** DTB_USER_CODE_START
 ***
 *** All necessary header files have been included.
 ***
 *** Add include files, types, macros, externs, and user functions here.
 ***/

#include <ab_private/obj_notify.h>
#include <ab_private/ab.h>
#include <ab_private/abobj.h>
#include <ab_private/abobj_set.h>
#include <ab_private/ab_utils.h>
#include <ab_private/proj.h>
#include <ab_private/prop.h>
#include <ab_private/conn.h>
#include <ab_private/appfw.h>

#define APPFW_I18N_KEY	123 /* needed for Checkbox initialization */
#define NO_DISMISS	0
#define DISMISS		1
#include "dtb_utils.h"

/*
 * AppFramework Editor Settings
 */
typedef struct  APPFW_EDITOR_SETTINGS
{
    Widget                      prop_sheet;
    PropFieldSettingRec		vendor;
    PropFieldSettingRec		version;
    PropObjOptionsSettingRec    rootwin;
    PropCheckboxSettingRec	i18n;
    PropCheckboxSettingRec	gencode_res_file;
    PropOptionsSettingRec       session_mgt;
    PropOptionsSettingRec       tooltalk;
} AppfwEditorSettingsRec, *AppfwEditorSettings;


static Widget	appfw_editor_init(
    		    DtbAppfwDialogInfo  appfw_cgen,
    		    Widget              parent
		);
static int      appfw_editor_load(
                    ABObj   project
                );
static int	appfw_editor_load_gencode(
			ABObj 			project,
			AppfwEditorSettings	afs
		);
static int      appfw_editor_apply(
                    ABObj   project
                );
static void	turnoff_changebars(
		);
static BOOL	rootwin_test_func(
		    ABObj	obj
		);
static BOOL	gencode_arg_class_is_supported(
		    AB_ARG_CLASS argClass
		);
static BOOL	appfw_editor_pending(
		);
static void	appfw_editor_prevent_closeCB(
		    Widget	w,
		    XtPointer	client_data,
		    XtPointer	call_data
		);
/*
 * Object-Callbacks
 */
static int      obj_renamedOCB(
                    ObjEvAttChangeInfo  info
                );
static int      obj_destroyedOCB(
                    ObjEvDestroyInfo    info
                );

static AppfwEditorSettingsRec appfw_editor_settings_rec;
static AppfwEditorSettingsRec *afs = &appfw_editor_settings_rec;

void
appfw_notify_new_project(
    ABObj	project
)
{
    if (AB_appfw_dialog != NULL)
   	appfw_editor_load(project);
}

void
appfw_show_dialog(
    ABObj	project
)
{
    /* If there is no App Framework Editor, create it */
    if (AB_appfw_dialog == (Widget)NULL)
    {
	AB_appfw_dialog = appfw_editor_init(&dtb_appfw_dialog, dtb_get_toplevel_widget());
    }
    if (AB_appfw_dialog != NULL)
    {
	appfw_editor_load(project);
	ab_show_window(AB_appfw_dialog);
    }

}

static Widget 
appfw_editor_init(
    DtbAppfwDialogInfo	appfw_cgen,
    Widget		parent
)
{
    Widget                      item[10];
    int                         item_val[10];
    int			   	n;

    /*
     * Create Dialog widgets...
     */
    dtbAppfwDialogInfo_clear(appfw_cgen);

    if (dtb_appfw_dialog_initialize(appfw_cgen, parent) == 0)
    {
     	/* 
	 * Hook widgets up to AppFramework Editor
      	 */
	afs->prop_sheet = appfw_cgen->ctrl_panel;

	/*************** 
	 * Application *
 	 ***************/
	/* Vendor */
	prop_field_init(&(afs->vendor), appfw_cgen->vendor_field_label,
		appfw_cgen->vendor_field, appfw_cgen->vendor_cb);

	/* Version */
	prop_field_init(&(afs->version), appfw_cgen->version_field_label,
		appfw_cgen->version_field, appfw_cgen->vendor_cb);

	/* Primary Main Window */
        n = 0;
        item[n] = appfw_cgen->rootwin_opmenu_items.None_item;
        item_val[n] = NULL; n++;
        prop_obj_options_init(&(afs->rootwin), appfw_cgen->rootwin_label,
                appfw_cgen->rootwin_opmenu, appfw_cgen->rootwin_opmenu_menu,
                n, item, (XtPointer*)item_val,
                appfw_cgen->rootwin_cb, True, /* Display module */
                NULL, rootwin_test_func);

	/* Need to monitor any changes in the Primary MainWindow object */
    	obj_add_rename_callback(obj_renamedOCB, "APPFW");
    	obj_add_destroy_callback(obj_destroyedOCB, "APPFW");

	/************************ 
	 * Internationalization *
	 ************************/

	/* Enabled */
        n = 0;
        item[n] = appfw_cgen->i18n_checkbox_items.nolabel_item;
        item_val[n] = APPFW_I18N_KEY; n++;
        prop_checkbox_init(&(afs->i18n), appfw_cgen->i18n_checkbox_label,
                appfw_cgen->i18n_checkbox, n, item, item_val,
                appfw_cgen->i18n_cb);


        /************************
         * Generated Code 	*
         ************************/

	n = 0;
   	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Colors_item;
	item_val[n++] = (int)AB_ARG_CLASS_COLOR;
	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Geometry_item;
	item_val[n++] = (int)AB_ARG_CLASS_GEOMETRY;
	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Label_Strings_item;
	item_val[n++] = (int)AB_ARG_CLASS_NON_GLYPH_LABEL;
	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Other_Strings_item;
	item_val[n++] = (int)AB_ARG_CLASS_OTHER_STRING;
	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Other_item;
	item_val[n++] = (int)AB_ARG_CLASS_OTHER;
	item[n] = dtb_appfw_dialog.gencode_res_checkbox_items.Initial_Values_item;
	item_val[n++] = (int)AB_ARG_CLASS_VALUE;
	prop_checkbox_init(&(afs->gencode_res_file), appfw_cgen->gencode_label,
		appfw_cgen->gencode_res_checkbox, n, item, item_val,
		appfw_cgen->gencode_res_cb);

	appfw_editor_load_gencode(proj_get_project(), afs);


	/********************** 
	 * Session Management *
	 **********************/

        n = 0;
        item[n] = appfw_cgen->ssmgmt_method_opmenu_items.None_item;
        item_val[n] = AB_SESSIONING_NONE; n++;
        item[n] = appfw_cgen->ssmgmt_method_opmenu_items.Command_Line_item;
        item_val[n] = AB_SESSIONING_CMDLINE; n++;
        item[n] = appfw_cgen->ssmgmt_method_opmenu_items.Session_File_item;
        item_val[n] = AB_SESSIONING_FILE; n++;
        item[n] = appfw_cgen->ssmgmt_method_opmenu_items.Both_item;
        item_val[n] = AB_SESSIONING_CMDLINE_AND_FILE; n++;
        prop_options_init(&(afs->session_mgt), 
		appfw_cgen->ssmgt_method_label,
                appfw_cgen->ssmgmt_method_opmenu, 
		appfw_cgen->ssmgmt_method_opmenu_menu,
                n, item, (XtPointer*)item_val,
                appfw_cgen->session_cb);

	/**************
	 *  ToolTalk  *
	 **************/

	/* Desktop Message Handling */
        n = 0;
        item[n] = appfw_cgen->tt_msg_handling_opmenu_items.None_item;
        item_val[n] = AB_TOOLTALK_NONE; n++;
        item[n] = appfw_cgen->tt_msg_handling_opmenu_items.Basic_item;
        item_val[n] = AB_TOOLTALK_DESKTOP_BASIC; n++;
        item[n] = appfw_cgen->tt_msg_handling_opmenu_items.Advanced_item;
        item_val[n] = AB_TOOLTALK_DESKTOP_ADVANCED; n++;
        prop_options_init(&(afs->tooltalk), 
		appfw_cgen->tt_msg_handling_label,
                appfw_cgen->tt_msg_handling_opmenu, 
		appfw_cgen->tt_msg_handling_opmenu_menu,
                n, item, (XtPointer*)item_val,
                appfw_cgen->tt_msg_handling_cb);


	/* Setup Ok, Apply, Reset, Cancel buttons */
	XtVaSetValues(appfw_cgen->ok_button, XmNuserData, 
			(XtArgVal)DISMISS, NULL);
	XtVaSetValues(appfw_cgen->cancel_button, XmNuserData, 
			(XtArgVal)DISMISS, NULL);
	XtVaSetValues(appfw_cgen->apply_button, XmNuserData, 
			(XtArgVal)NO_DISMISS, NULL);
	XtVaSetValues(appfw_cgen->reset_button, XmNuserData, 
			(XtArgVal)NO_DISMISS, NULL);

	/* Setup dialog to participate in dtbuilder window
	 * protocol
	 */
	ab_register_window(appfw_cgen->dialog_shellform,
		AB_WIN_DIALOG, WindowHidden, 
		AB_toplevel, AB_WPOS_STACK_CENTER,
		appfw_editor_prevent_closeCB, NULL);

        prop_changebars_cleared(afs->prop_sheet);

        return (dtb_appfw_dialog.dialog_shellform);
    }
    else
    {
	util_dprintf(3, "appfw_show_dialog: could not initialize Application Framework Editor\n");
     	return NULL;
    }
}


static int
appfw_editor_load(
    ABObjPtr 	project
)
{
    ABObj		rootwin;
 
    if (project == NULL || !obj_is_project(project))
        return ERROR;

    /***************
     * Application *
     ***************/
    prop_field_set_value(&(afs->vendor), obj_get_vendor(project), False);
    prop_field_set_value(&(afs->version), obj_get_version(project), False);

    prop_obj_options_load(&(afs->rootwin), project);
    rootwin = obj_get_root_window(project);
    prop_obj_options_set_value(&(afs->rootwin), rootwin, False);

    /************************
     * Internationalization *
     ************************/
    prop_checkbox_set_value(&(afs->i18n), APPFW_I18N_KEY, 
		obj_get_i18n_enabled(project), False);

    /************************ 
     * Generated Code       *
     ************************/
    appfw_editor_load_gencode(project, afs);


    /**********************
     * Session Management *
     **********************/
    prop_options_set_value(&(afs->session_mgt), 
		(XtPointer)obj_get_sessioning_method(project), False);


    /**************
     *  ToolTalk  *
     **************/
    prop_options_set_value(&(afs->tooltalk),
		(XtPointer)obj_get_tooltalk_level(project), False);

    turnoff_changebars();

    return OK;
}


/*
 * If project is NULL, all settings are turned off
 */
static int
appfw_editor_load_gencode(
			ABObj			project,
			AppfwEditorSettings	afs
)
{
	/* get resource "classes" that go to resource file */
	AB_ARG_CLASS_FLAGS	resArgClasses = AB_ARG_CLASS_FLAGS_NONE;
	AB_ARG_CLASS		argClass;
	AB_ARG_CLASS_FLAGS	argClassFlag;
	BOOL			argIsResFile;
	int			i = 0;

	if (project != NULL)
	{
	    resArgClasses = obj_get_res_file_arg_classes(project);
	}

	for (i = ((int)AB_ARG_CLASS_UNDEF)+1;
	     i < (int)AB_ARG_CLASS_NUM_VALUES;
	     ++i)
	{
	    argClass = (AB_ARG_CLASS)i;
	    if (!gencode_arg_class_is_supported(argClass))
	    {
		continue;
	    }
	    argClassFlag = util_arg_class_to_flag(argClass);
	    argIsResFile = ((resArgClasses & argClassFlag) != 0);
	    prop_checkbox_set_value(&(afs->gencode_res_file),
		argClass, argIsResFile, FALSE);
	}
    return 0;
}


static int
appfw_editor_apply(
    ABObjPtr 	project
)
{
    ABObj	old_rootwin, rootwin;
    STRING	new_value;

    if (project == NULL || !obj_is_project(project))
        return ERROR;

    /***************
     * Application *
     ***************/
    if (prop_changed(afs->vendor.changebar))
    {
	STRING	new_vendor, new_version;

	new_value = prop_field_get_value(&(afs->vendor));
	if (util_strcmp(new_value, obj_get_vendor(project)) != 0)
	{
	    obj_set_vendor(project, new_value);
            abobj_set_save_needed(project, True);
	}
	util_free(new_value);

        new_value = prop_field_get_value(&(afs->version));
        if (util_strcmp(new_value, obj_get_version(project)) != 0)
        {
            obj_set_version(project, new_value);
            abobj_set_save_needed(project, True);
        }
        util_free(new_value); 
    }

    if (prop_changed(afs->rootwin.options.changebar))
    {
	rootwin = prop_obj_options_get_value(&(afs->rootwin));

    	old_rootwin = obj_get_root_window(project);

    	if (old_rootwin != rootwin)
    	{
            abobj_set_save_needed(project, True); 
            if (old_rootwin != NULL)
            	obj_set_class_name(old_rootwin, _topLevelShell);
 
            obj_set_root_window(project, rootwin);
 
            if (rootwin != NULL)
            	obj_set_class_name(rootwin, _applicationShell);
    	}
    }
 
    /************************
     * Internationalization *
     ************************/
    if (prop_changed(afs->i18n.changebar))
    {
        abobj_set_i18n_enabled(project,
            (BOOL)prop_checkbox_get_value(&(afs->i18n), APPFW_I18N_KEY));
    }

    /************************  
     * Generated Code       * 
     ************************/ 
    if (prop_changed(afs->gencode_res_file.changebar))
    {
        AB_ARG_CLASS_FLAGS	resArgClasses = AB_ARG_CLASS_FLAGS_NONE;
	AB_ARG_CLASS		argClass;
	int	i;

	for (i = ((int)AB_ARG_CLASS_UNDEF)+1;
	     i < (int)AB_ARG_CLASS_NUM_VALUES;
	     ++i)
	{
	    argClass = (AB_ARG_CLASS)i;
	    argClass = (AB_ARG_CLASS)i;
	    if (!gencode_arg_class_is_supported(argClass))
	    {
		continue;
	    }
	    if (prop_checkbox_get_value(&(afs->gencode_res_file), argClass))
	    {
	        resArgClasses |= util_arg_class_to_flag(argClass);
	    }
	}

        obj_set_res_file_arg_classes(project, resArgClasses);
	abobj_set_save_needed(project, TRUE);
    }
 
 
    /**********************
     * Session Management *
     **********************/
    if (prop_changed(afs->session_mgt.changebar))
    {
        abobj_set_sessioning_method(project,
            (AB_SESSIONING_METHOD)prop_options_get_value(&(afs->session_mgt)));

            /* REMIND: Need to Check that Connections match Session Method */ 
    }
 
    /**************
     *  ToolTalk  *
     **************/
    if (prop_changed(afs->tooltalk.changebar))
    {
	AB_TOOLTALK_LEVEL	new_level;

	new_level = (AB_TOOLTALK_LEVEL)prop_options_get_value(&(afs->tooltalk));
    	if (new_level != obj_get_tooltalk_level(project))
        {
            obj_set_tooltalk_level(project, new_level);
            abobj_set_save_needed(project, True);

	    /* REMIND: Need to Check that Connections match Tooltalk level */
        }   
    }

    turnoff_changebars();

    return OK;

}

static void
turnoff_changebars(
)
{

    prop_set_changebar(afs->vendor.changebar,    	PROP_CB_OFF);
    prop_set_changebar(afs->rootwin.options.changebar,  PROP_CB_OFF);
    prop_set_changebar(afs->i18n.changebar,		PROP_CB_OFF);
    prop_set_changebar(afs->gencode_res_file.changebar,	PROP_CB_OFF);
    prop_set_changebar(afs->tooltalk.changebar,		PROP_CB_OFF);
    prop_set_changebar(afs->session_mgt.changebar,	PROP_CB_OFF);

    prop_changebars_cleared(afs->prop_sheet);

}


static BOOL
gencode_arg_class_is_supported(AB_ARG_CLASS argClass)
{
    switch (argClass)
    {
	case AB_ARG_CLASS_COLOR:
	case AB_ARG_CLASS_GEOMETRY:
	case AB_ARG_CLASS_NON_GLYPH_LABEL:
	case AB_ARG_CLASS_OTHER_STRING:
	case AB_ARG_CLASS_OTHER:
	case AB_ARG_CLASS_VALUE:
	    return TRUE;
    }
    return FALSE;
}

/*
 * Test whether an object can be the "Primary Main Window" 
 * for the Application
 */
static BOOL
rootwin_test_func(
    ABObj       obj
)
{
    return(obj_is_base_win(obj));
}

static int
obj_renamedOCB(
    ObjEvAttChangeInfo    info
)
{
    ABObj	project = proj_get_project();
    STRING	modname;

    if (obj_is_base_win(info->obj) && 
	obj_get_root_window(project) == info->obj)
    {
    	/* Reset Primary MainWindow OptionMenu to display new name */
	modname = abobj_get_moduled_name(info->obj);
	ui_optionmenu_change_label(afs->rootwin.options.optionbox, 
		modname);
	util_free(modname);
	/* Need to force the Project to save the new Primary Window name */
        abobj_set_save_needed(project, True);
    }
    return 0;
}

static int
obj_destroyedOCB(
    ObjEvDestroyInfo    info
)
{
    ABObj       project = proj_get_project();

    if (obj_is_base_win(info->obj) && 
        obj_get_root_window(project) == info->obj)
    {
    	/* Reset Primary MainWindow OptionMenu to display None */
    	prop_obj_options_set_value(&(afs->rootwin), NULL, False);
        /* Need to force the Project to save no Primary Window */ 
        abobj_set_save_needed(project, True); 
    }

    return 0;
}

/*
 * Returns TRUE if there are edits pending on
 * the Application Framework Editor
 */
static BOOL
appfw_editor_pending(
)
{
    if (afs && prop_changebars_pending(afs->prop_sheet))
	return TRUE;
    
    return FALSE;
}

/*
 * App Framework Editor window close callback
 */
static void
appfw_editor_prevent_closeCB(
    Widget	widget,
    XtPointer	client_data,
    XtPointer	call_data
)
{
    DTB_MODAL_ANSWER	answer = DTB_ANSWER_NONE;

    /*
     * If there are unapplied edits...
     */
    if (appfw_editor_pending())
    {
    	if (dtb_app_resource_rec.implied_apply == True)
            answer = DTB_ANSWER_ACTION1;
    	else
    	{   
	    DtbObjectHelpData	help_data = NULL;
	    XmString		xm_buf;
	    char		*msg = NULL,
				*help_text = NULL;
	    /*
	     * Initialize warning message object if necessary
	     */
	    dtb_appfw_appfw_pending_msg_initialize(&dtb_appfw_appfw_pending_msg);

	    /*
	     * Malloc/setup struct for help text
	     */
	    help_data = (DtbObjectHelpData) util_malloc(sizeof(DtbObjectHelpDataRec));

	    xm_buf = XmStringCreateLocalized(catgets(Dtb_project_catd, 100, 248, "Properties for your project have been modified on the\nApplication Framework Editor but not Applied.\nApply Changes or Cancel Close operation."));

	    help_data->help_volume = help_data->help_locationID = NULL;
	    help_data->help_text = XtNewString(catgets(Dtb_project_catd, 100, 249, "Click Apply Changes to apply the changes \nand close the Application Framework Editor.\n\nClick Cancel if you don't want to apply the\nchanges and want the Application Framework\nEditor to remain displayed. You can then click\nReset to undo the changes before closing\nthe Application Framework Editor."));

	    /*
	     * Pop up warning message dialog
	     */
	    answer = dtb_show_modal_message(AB_appfw_dialog, &dtb_appfw_appfw_pending_msg,
					xm_buf, help_data, NULL);
            /*
             * Free XmString, help text/struct
             */
            XmStringFree(xm_buf);
            XtFree(help_data->help_text);
            util_free(help_data);
	}
	/*
	 * Response was "Apply Changes"
	 */
	if (answer == DTB_ANSWER_ACTION1)
	{
	    /*
	     * Do Apply and pop down dialog
	     */
            appfw_editor_apply(proj_get_project());
            ui_win_show(AB_appfw_dialog, False, XtGrabNone);
	}
    }
    else
	/*
	 * Pop down dialog if no edits pending
	 */
        ui_win_show(AB_appfw_dialog, False, XtGrabNone);
}

/*** DTB_USER_CODE_END
 ***
 *** End of user code section
 ***
 **************************************************************************/



void 
appfw_ssmgmt_method_opmenu_None_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_save_cb_button, False);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_None_item_CB2(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_restore_cb_button, False);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Command_Line_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_save_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Command_Line_item_CB2(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_restore_cb_button, False);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Session_File_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_save_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Session_File_item_CB2(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_restore_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Both_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_save_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_ssmgmt_method_opmenu_Both_item_CB2(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->ssmgt_restore_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_tt_msg_handling_opmenu_None_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->tt_msg_cb_button, False);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_tt_msg_handling_opmenu_Basic_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->tt_msg_cb_button, False);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_tt_msg_handling_opmenu_Advanced_item_CB1(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    DtbAppfwDialogInfo	instance = (DtbAppfwDialogInfo)clientData;
    
    if (!(instance->initialized))
    {
        dtb_appfw_dialog_initialize(instance, dtb_palette_ab_palette_main.ab_palette_main);
    }

    XtSetSensitive(instance->tt_msg_cb_button, True);
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_applyCB(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/

    ABObj	project = proj_get_project();
    XtArgVal	dismiss;
    
    if (!project)
	return;

    appfw_editor_apply(project);

    XtVaGetValues(widget, XmNuserData, &dismiss, NULL);
    if (dismiss)
	ui_win_show(widget, False, XtGrabNone);

    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_resetCB(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/

    ABObj       project = proj_get_project();
    XtArgVal    dismiss;

    if (!project)
        return;
    
    appfw_editor_load(project);

    XtVaGetValues(widget, XmNuserData, &dismiss, NULL);
    if (dismiss)
	ui_win_show(widget, False, XtGrabNone);

    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_invoke_connCB(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/

    AB_WHEN     when = AB_WHEN_UNDEF;

    conn_set_source(proj_get_project());
    conn_set_target(NULL);
    XtVaGetValues(widget, XmNuserData, &when, NULL);     
    conn_override_default_when(when);
    conn_popup_dialog(dtb_get_toplevel_widget(), (XtPointer)0, NULL);
    conn_reset_default_when();

    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_register_save_btn(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    DtbAppfwDialogInfo	dtbSource = (DtbAppfwDialogInfo)callData;
    
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/

    XtVaSetValues(widget, XmNuserData, AB_WHEN_SESSION_SAVE, NULL);

    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_register_restore_btn(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    DtbAppfwDialogInfo	dtbSource = (DtbAppfwDialogInfo)callData;
    
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/

    XtVaSetValues(widget, XmNuserData, AB_WHEN_SESSION_RESTORE, NULL);

    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}


void 
appfw_register_ttalk_btn(
    Widget widget,
    XtPointer clientData,
    XtPointer callData
)
{
    DtbAppfwDialogInfo	dtbSource = (DtbAppfwDialogInfo)callData;
    
    /*** DTB_USER_CODE_START vvv Add C variables and code below vvv ***/
    /*** DTB_USER_CODE_END   ^^^ Add C variables and code above ^^^ ***/
    
    /*** DTB_USER_CODE_START vvv Add C code below vvv ***/

    XtVaSetValues(widget, XmNuserData, AB_WHEN_TOOLTALK_DO_COMMAND, NULL);

    /*** DTB_USER_CODE_END   ^^^ Add C code above ^^^ ***/
}



/**************************************************************************
 *** DTB_USER_CODE_START
 ***
 *** All automatically-generated data and functions have been defined.
 ***
 *** Add new functions here, or at the top of the file.
 ***/


/*** DTB_USER_CODE_END
 ***
 *** End of user code section
 ***
 **************************************************************************/



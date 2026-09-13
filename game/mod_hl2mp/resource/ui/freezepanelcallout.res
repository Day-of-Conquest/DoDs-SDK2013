"Resource/UI/FreezePanelCallout.res"
{
	"CalloutBG"
	{
		"ControlName"		"ScalableImagePanel"
		"fieldName"		"CalloutBG"
		"xpos"			"10"
		"ypos"			"10"
		"zpos"			"0"
		"wide"			"80"
		"tall"			"30"
		"visible"		"1"
		"enabled"		"1"
		"image"			"../hud/tournament_panel_brown"
		"scaleImage"		"1"
		
		"src_corner_height"	"23"				// pixels inside the image
		"src_corner_width"	"23"
		
		"draw_corner_width"	"8"				// screen size of the corners ( and sides ), proportional
		"draw_corner_height" 	"8"	
	}
	
	"ArrowIcon"
	{
		"ControlName"		"ImagePanel"
		"fieldName"		"ArrowIcon"
		"xpos"			"20"
		"ypos"			"34"
		"zpos"			"3"
		"wide"			"20"
		"tall"			"10"
		"visible"		"1"
		"enabled"		"1"
		"image"			"../hud/freezecam_callout_arrow"
		"scaleImage"	"1"	
	}	
		
	"CalloutLabel"
	{	
		"ControlName"		"Label"
		"fieldName"		"CalloutLabel"
		"font"			"HudFontSmallest"
		"xpos"			"15"
		"ypos"			"15"
		"zpos"			"2"
		"wide"			"70"
		"tall"			"20"
		"autoResize"		"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"labelText"		"#FreezePanel_Callout"
		"textAlignment"		"center"
		"dulltext"		"0"
		"brighttext"		"0"
	}
}

"Resource/UI/FreezePanel_Basic.res"
{
	"FreezePanelBase"
	{
		"ControlName"	"EditablePanel"
		"fieldName"		"FreezePanelBase"
		"xpos"			"r270"
		"ypos"			"250"		
		"wide"			"267"		
		"tall"			"120"		
		"visible"		"1"
		
		"PlayerStatusHealth"
		{
			"ControlName"	"CDoDHudHealth"	
			"fieldName"		"PlayerStatusHealth"
			"xpos"			"0"
			"ypos"			"30"
			"zpos"			"1"
			"wide"			"46"
			"tall"			"78"
			"visible"		"1"
			"enabled"		"1"
		}	
	
		"FreezePanelBG"
		{
			"ControlName"	"ScalableImagePanel"
			"fieldName"		"FreezePanelBG"
			"xpos"			"20"
			"ypos"			"57"
			"zpos"			"0"
			"wide"			"246"
			"tall"			"43"
			"visible"		"1"
			"enabled"		"1"
			"image"			"../hud/tournament_panel_brown"
			"scaleImage"	"1"
			
			"src_corner_height"	"23"				// pixels inside the image
			"src_corner_width"	"23"
			
			"draw_corner_width"	"8"				// screen size of the corners ( and sides ), proportional
			"draw_corner_height" 	"8"	
		}
				
		"FreezeLabel"
		{	
			"ControlName"		"Label"
			"fieldName"		"FreezeLabel"
			"font"			"DefaultSmall"
			"xpos"			"50"
			"ypos"			"62"
			"zpos"			"1"
			"wide"			"252"
			"tall"			"12"
			"autoResize"		"0"
			"pinCorner"		"0"
			"visible"		"1"
			"enabled"		"1"
			"labelText"		"#FreezePanel_Killer"
			"textAlignment"		"North-West"
			"dulltext"		"0"
			"brighttext"		"0"
		}
				
		"FreezeLabelKiller"
		{	
			"ControlName"	"Label"
			"fieldName"		"FreezeLabelKiller"
			"font"			"HudFontSmall"
			"xpos"			"81"
			"ypos"			"73"
			"zpos"			"1"
			"wide"			"252"
			"tall"			"18"
			"autoResize"	"0"
			"pinCorner"		"0"
			"visible"		"1"
			"enabled"		"1"
			"labelText"		"%killername%"
			"textAlignment"		"West"
		}
		
		"AvatarImage"
		{
			"ControlName"		"CAvatarImagePanel"
			"fieldName"		"AvatarImage"
			"xpos"			"60"
			"ypos"			"73"
			"zpos"			"0"
			"wide"			"14"
			"tall"			"14"
			"visible"		"1"
			"enabled"		"1"
			"image"			""
			"scaleImage"		"1"	
			"color_outline"		"52 48 45 255"
		}	
		
		"NemesisSubPanel"
		{
			"ControlName"		"EditablePanel"
			"fieldName"		"NemesisSubPanel"
			"xpos"			"0"	
			"ypos"			"30"
			"zpos"			"4"
			"wide"			"306"
			"tall"			"38"
			"visible"		"1"

			"NemesisPanelBG"
			{
				"ControlName"		"ScalableImagePanel"
				"fieldName"		"NemesisPanelBG"
				"xpos"			"90"
				"ypos"			"0"
				"zpos"			"5"
				"wide"			"166"
				"tall"			"38"
				"visible"		"1"
				"enabled"		"1"
				"image"			"../hud/tournament_panel_brown"
				"scaleImage"		"1"	
				
				"src_corner_height"	"23"				// pixels inside the image
				"src_corner_width"	"23"
				
				"draw_corner_width"	"8"				// screen size of the corners ( and sides ), proportional
				"draw_corner_height" 	"8"	
			}
			
			"NemesisIcon"
			{
				"ControlName"	"ImagePanel"
				"fieldName"		"NemesisIcon"
				"xpos"			"95"
				"ypos"			"-1"
				"zpos"			"6"
				"wide"			"36"
				"tall"			"36"
				"visible"		"1"
				"enabled"		"1"
				"image"			"../hud/leaderboard_nemesis_freezecam"
				"scaleImage"	"1"	
			}		
			
			"NemesisLabel"
			{	
				"ControlName"		"Label"
				"fieldName"		"NemesisLabel"
				"font"			"HudFontMediumSmall"
				"xpos"			"134"
				"ypos"			"10"
				"zpos"			"6"
				"wide"			"112"
				"tall"			"18"
				"autoResize"		"0"
				"pinCorner"		"0"
				"visible"		"1"
				"enabled"		"1"
				"labelText"		"%nemesisname%"
				"textAlignment"		"west"
			}
		}
	}
		
	"ScreenshotPanel"
	{
		"ControlName"	"EditablePanel"
		"fieldName"		"ScreenshotPanel"
		"xpos"			"c-83"
		"ypos"			"-50"
		"zpos"			"2"
		"wide"			"166"
		"tall"			"45"
		"visible"		"0"
			
		"ScreenshotPanelBG"
		{
			"ControlName"		"ScalableImagePanel"
			"fieldName"		"ScreenshotPanelBG"
			"xpos"			"0"
			"ypos"			"8"
			"zpos"			"0"
			"wide"			"166"
			"tall"			"38"
			"visible"		"1"
			"enabled"		"1"			
			"image"			"../hud/tournament_panel_brown"
			"scaleImage"	"1"
			
			"src_corner_height"		"23"				// pixels inside the image
			"src_corner_width"		"23"
			
			"draw_corner_width"		"8"				// screen size of the corners ( and sides ), proportional
			"draw_corner_height" 	"8"	
		}
		
		"ScreenshotIcon"
		{
			"ControlName"		"ImagePanel"
			"fieldName"		"ScreenshotIcon"
			"xpos"			"3"
			"ypos"			"0"
			"zpos"			"1"
			"wide"			"36"
			"tall"			"36"
			"visible"		"1"
			"enabled"		"1"
			"image"			"../hud/ico_camera"
			"scaleImage"	"1"	
		}		
		"ScreenshotLabel"
		{	
			"ControlName"		"Label"
			"fieldName"		"ScreenshotLabel"
			"font"			"SpectatorKeyHints"
			"xpos"			"40"
			"ypos"			"25"
			"zpos"			"2"
			"wide"			"133"
			"tall"			"12"
			"autoResize"		"0"
			"pinCorner"		"0"
			"visible"		"1"
			"enabled"		"1"
			"labelText"		"%text%"
			"textAlignment"		"west"
			"dulltext"		"0"
			"brighttext"		"0"
		}
	}	
}

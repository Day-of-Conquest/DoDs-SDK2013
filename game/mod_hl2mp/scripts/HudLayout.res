"Resource/HudLayout.res"
{
	HudHealth [!$DECK]
	{
		"fieldName"		"HudHealth"
		"xpos"	"16"
		"ypos"	"432"
		"wide"	"102"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "20"
		"digit_xpos" "50"
		"digit_ypos" "2"
	}
	HudHealth [$DECK]
	{
		"fieldName"		"HudHealth"
		"xpos"	"16"
		"ypos"	"426"
		"wide"	"130"
		"tall"  "42"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "23"
		"digit_xpos" "66"
		"digit_ypos" "0"
	}

	TargetID
	{
		"fieldName" "TargetID"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	TeamDisplay
	{
		"fieldName" "TeamDisplay"
	    "visible" "0"
	    "enabled" "1"
		"xpos"	"16"
		"ypos"	"410" [!$DECK]
		"ypos"	"400" [$DECK]
	    "wide" "200"
	    "tall" "60"
	    "text_xpos" "8"
	    "text_ypos" "4"
	}

	HudVoiceSelfStatus
	{
		"fieldName" "HudVoiceSelfStatus"
		"visible" "1"
		"enabled" "1"
		"xpos" "r43"
		"ypos" "355"
		"wide" "24"
		"tall" "24"
	}

	HudVoiceStatus
	{
		"fieldName" "HudVoiceStatus"
		"visible" "1"
		"enabled" "1"
		"xpos" "r145"
		"ypos" "0"
		"wide" "145"
		"tall" "400"

		"item_wide"	"135"

		"show_avatar"		"0"

		"show_dead_icon"	"1"
		"dead_xpos"			"1"
		"dead_ypos"			"0"
		"dead_wide"			"16"
		"dead_tall"			"16"

		"show_voice_icon"	"1"
		"icon_ypos"			"0"
		"icon_xpos"			"15"
		"icon_tall"			"16"
		"icon_wide"			"16"

		"text_xpos"			"33"
	}

	HudSuit [!$DECK]
	{
		"fieldName"		"HudSuit"
		"xpos"	"140"
		"ypos"	"432"
		"wide"	"108"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "20"
		"digit_xpos" "50"
		"digit_ypos" "2"
	}
	HudSuit [$DECK]
	{
		"fieldName"		"HudSuit"
		"xpos"	"150"
		"ypos"	"426"
		"wide"	"120"
		"tall"  "42"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "23"
		"digit_xpos" "56"
		"digit_ypos" "0"
	}


	HudAmmo	[!$DECK]
	{
		"fieldName" "HudAmmo"
		"xpos"	"r150"
		"ypos"	"432"
		"wide"	"136"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "20"
		"digit_xpos" "44"
		"digit_ypos" "2"
		"digit2_xpos" "98"
		"digit2_ypos" "16"
	}
	HudAmmo	[$DECK]
	{
		"fieldName" "HudAmmo"
		"xpos"	"r150"
		"ypos"	"426"
		"wide"	"152"
		"tall"  "42"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "24"
		"digit_xpos" "55"
		"digit_ypos" "0"
		"digit2_xpos" "100"
		"digit2_ypos" "12"
	}

	HudAmmoSecondary [!$DECK]
	{
		"fieldName" "HudAmmoSecondary"
		"xpos"	"r76"
		"ypos"	"432"
		"wide"	"60"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "22"
		"digit_xpos" "36"
		"digit_ypos" "2"
	}
	HudAmmoSecondary [$DECK]
	{
		"fieldName" "HudAmmoSecondary"
		"xpos"	"r82"
		"ypos"	"426"
		"wide"	"70"
		"tall"  "42"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"text_xpos" "8"
		"text_ypos" "24"
		"digit_xpos" "42"
		"digit_ypos" "0"
	}

	HudSuitPower [!$DECK]
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"396"
		"wide"	"102"
		"tall"	"26"

		"AuxPowerLowColor" "255 0 0 220"
		"AuxPowerHighColor" "255 220 0 220"
		"AuxPowerDisabledAlpha" "70"

		"BarInsetX" "8"
		"BarInsetY" "15"
		"BarWidth" "92"
		"BarHeight" "4"
		"BarChunkWidth" "6"
		"BarChunkGap" "3"

		"text_xpos" "8"
		"text_ypos" "4"
		"text2_xpos" "8"
		"text2_ypos" "22"
		"text2_gap" "10"

		"PaintBackgroundType"	"2"
	}

	HudSuitPower	[$DECK]
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"386"
		"wide"	"112"
		"tall"	"54"

		"AuxPowerLowColor" "255 0 0 220"
		"AuxPowerHighColor" "255 220 0 220"
		"AuxPowerDisabledAlpha" "70"

		"BarInsetX" "8"
		"BarInsetY" "18"
		"BarWidth" "102"
		"BarHeight" "5"
		"BarChunkWidth" "6"
		"BarChunkGap" "3"

		"text_xpos" "8"
		"text_ypos" "4"
		"text2_xpos" "8"
		"text2_ypos" "26"
		"text2_gap" "14"

		"PaintBackgroundType"	"2"
	}

	HudPosture	[$WIN32]
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"16"
		"ypos"	"316"
		"tall"  "35"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"8"
		"icon_ypos" 	"-2"
	}
	HudPosture	[$X360]
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"48"
		"ypos"	"316"
		"tall"  "36"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"10"
		"icon_ypos" 	"2"
	}

	HudFlashlight
	{
		"fieldName" "HudFlashlight"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"270"		[$WIN32]
		"ypos"	"444"		[!$DECK]
		"ypos"	"436"		[$DECK]
		"xpos_hidef"	"306"		[$X360]		// aligned to left
		"xpos_lodef"	"c-18"		[$X360]		// centered in screen
		"ypos"	"428"		[$X360]
		"tall"  "24" [!$DECK]
		"tall"  "30" [$DECK]
		"wide"	"36" [!$DECK]
		"wide"	"46" [$DECK]
		"font"	"WeaponIconsSmall" [!$DECK]
		"font"	"FlashlightDeck" [$DECK]

		"icon_xpos"	"4"
		"icon_ypos" "-8" [!$DECK]
		"icon_ypos" "-12"  [$DECK]

		"BarInsetX" "4"
		"BarInsetY" "18" [!$DECK]
		"BarInsetY" "22" [$DECK]
		"BarWidth" "28" [!$DECK]
		"BarWidth" "36" [$DECK]
		"BarHeight" "2" [!$DECK]
		"BarChunkWidth" "2" [!$DECK]
		"BarHeight" "3" [$DECK]
		"BarChunkWidth" "3" [$DECK]
		"BarChunkGap" "1"
	}
	HudDamageIndicator
	{
		"fieldName" "HudDamageIndicator"
		"visible" "1"
		"enabled" "1"
		"DmgColorLeft" "255 0 0 0"
		"DmgColorRight" "255 0 0 0"

		"dmg_xpos" "30"
		"dmg_ypos" "100"
		"dmg_wide" "36"
		"dmg_tall1" "240"
		"dmg_tall2" "200"
	}

	HudZoom
	{
		"fieldName" "HudZoom"
		"visible" "1"
		"enabled" "1"
		"Circle1Radius" "66"
		"Circle2Radius"	"74"
		"DashGap"	"16"
		"DashHeight" "4"	[$WIN32]
		"DashHeight" "6"	[$X360]
		"BorderThickness" "88"
	}
	HudWeaponSelection
	{
		"fieldName" "HudWeaponSelection"
		"ypos" 	"16"	[$WIN32]
		"ypos" 	"32"	[$X360]
		"visible" "1"
		"enabled" "1"
		"SmallBoxSize" "32"
		"MediumBoxWide"	"95"
		"MediumBoxWide_hidef"	"78"
		"MediumBoxTall"	"50"
		"MediumBoxTall_hidef"	"50"
		"MediumBoxWide_lodef"	"74"
		"MediumBoxTall_lodef"	"50"
		"LargeBoxWide" "112"
		"LargeBoxTall" "84"
		"BoxGap" "8"
		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"
		"TextYPos" "64"
	}

	HudCrosshair
	{
		"fieldName" "HudCrosshair"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudDeathNotice
	{
		"fieldName" "HudDeathNotice"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudVehicle
	{
		"fieldName" "HudVehicle"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	ScorePanel
	{
		"fieldName" "ScorePanel"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudTrain
	{
		"fieldName" "HudTrain"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMOTD
	{
		"fieldName" "HudMOTD"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMessage
	{
		"fieldName" "HudMessage"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudMenu
	{
		"fieldName" "HudMenu"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudCloseCaption
	{
		"fieldName" "HudCloseCaption"
		"visible"	"1"
		"enabled"	"1"
		"xpos"		"c-250"
		"ypos"		"276"	[$WIN32]
		"ypos"		"236"	[$X360]
		"wide"		"500"
		"tall"		"136"	[$WIN32]
		"tall"		"176"	[$X360]

		"BgAlpha"	"128"

		"GrowTime"		"0.25"
		"ItemHiddenTime"	"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"	"0.15"	// Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"	"0.3"
		"topoffset"		"0"		[$WIN32]
		"topoffset"		"0"	[$X360]
	}

	HudChat
	{
		"fieldName" "HudChat"
		"visible" "0"
		"enabled" "1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	 "4"
		"tall"	 "4"
	}

	HudHistoryResource	[$WIN32]
	{
		"fieldName" "HudHistoryResource"
		"visible" "1"
		"enabled" "1"
		"xpos"	"r272" [$DECK]
		"xpos"	"r252" [!$DECK]
		"ypos"	"40"
		"wide"	 "248"
		"tall"	 "320"

		"history_gap"	"64" [$DECK]
		"history_gap"	"56" [!$DECK]
		"icon_inset"	"38"
		"text_inset"	"36"
		"text_inset"	"26"
		"NumberFont"	"HudNumbersSmall"
	}

	HudGeiger
	{
		"fieldName" "HudGeiger"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HUDQuickInfo
	{
		"fieldName" "HUDQuickInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudWeapon
	{
		"fieldName" "HudWeapon"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}
	HudAnimationInfo
	{
		"fieldName" "HudAnimationInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudPredictionDump
	{
		"fieldName" "HudPredictionDump"
		"visible" "1"
		"enabled" "1"
		"wide"	 "3840"
		"tall"	 "1080"
	}

	HudHintDisplay
	{
		"fieldName"				"HudHintDisplay"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"c-240"
		"ypos"					"c60"
		"xpos"	"r148"	[$X360]
		"ypos"	"r338"	[$X360]
		"wide"					"480"
		"tall"					"100"
		"HintSize"				"1"
		"text_xpos"				"8"
		"text_ypos"				"8"
		"center_x"				"0"	// center text horizontally
		"center_y"				"-1"	// align text on the bottom
		"paintbackground"		"0"
	}

	HudHintKeyDisplay
	{
		"fieldName"	"HudHintKeyDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"	[$WIN32]
		"ypos"		"r340"	[$WIN32]
		"xpos"		"r148"	[$X360]
		"ypos"		"r338"	[$X360]
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}


	HudSquadStatus	[!$DECK]
	{
		"fieldName"	"HudSquadStatus"
		"visible"	"1"
		"enabled" "1"
		"xpos"	"r120"
		"ypos"	"380"
		"wide"	"104"
		"tall"	"46"
		"text_xpos"	"8"
		"text_ypos"	"34"
		"SquadIconColor"	"255 220 0 160"
		"IconInsetX"	"8"
		"IconInsetY"	"0"
		"IconGap"		"24"

		"PaintBackgroundType"	"2"
	}
	HudSquadStatus	[$DECK]
	{
		"fieldName"	"HudSquadStatus"
		"visible"	"1"
		"enabled" "1"
		"xpos"	"r160"
		"ypos"	"372"
		"wide"	"144"
		"tall"	"46"
		"text_xpos"	"8"
		"text_ypos"	"28"
		"SquadIconColor"	"255 220 0 160"
		"IconInsetX"	"8"
		"IconInsetY"	"-10"
		"IconGap"		"39"

		"PaintBackgroundType"	"2"
	}


	HudPoisonDamageIndicator	[!$DECK]
	{
		"fieldName"	"HudPoisonDamageIndicator"
		"visible"	"0"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"338"
		"wide"	"136"
		"tall"	"38"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_ygap" "14"
		"TextColor"	"255 170 0 220"
		"PaintBackgroundType"	"2"
	}
	HudPoisonDamageIndicator	[$DECK]
	{
		"fieldName"	"HudPoisonDamageIndicator"
		"visible"	"0"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"262"
		"wide"	"190"
		"tall"	"42"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_ygap" "14"
		"TextColor"	"255 170 0 220"
		"PaintBackgroundType"	"2"
	}
	HudCredits
	{
		"fieldName"	"HudCredits"
		"TextFont"	"Default"
		"visible"	"1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"	"480"
		"TextColor"	"255 255 255 192"

	}

	HUDAutoAim
	{
		"fieldName" "HUDAutoAim"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"	[$WIN32]
		"tall"	 "480"	[$WIN32]
		"wide"	 "960"	[$X360]
		"tall"	 "720"	[$X360]
	}

	HudCommentary
	{
		"fieldName" "HudCommentary"
		"xpos"	"c-190"
		"ypos"	"350"
		"wide"	"380"
		"tall"  "40"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"2"

		"bar_xpos"		"50"
		"bar_ypos"		"20"
		"bar_height"	"8"
		"bar_width"		"320"
		"speaker_xpos"	"50"
		"speaker_ypos"	"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"	"8"

		"icon_texture"	"vgui/hud/icon_commentary"
		"icon_xpos"		"0"
		"icon_ypos"		"0"
		"icon_width"	"40"
		"icon_height"	"40"
	}

	HudHDRDemo
	{
		"fieldName" "HudHDRDemo"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"

		"Alpha"	"255"
		"PaintBackgroundType"	"2"

		"BorderColor"	"0 0 0 255"
		"BorderLeft"	"16"
		"BorderRight"	"16"
		"BorderTop"		"16"
		"BorderBottom"	"64"
		"BorderCenter"	"0"

		"TextColor"		"255 255 255 255"
		"LeftTitleY"	"422"
		"RightTitleY"	"422"
	}

	AchievementNotificationPanel
	{
		"fieldName"				"AchievementNotificationPanel"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"180"
		"wide"					"f10"	[$WIN32]
		"wide"					"f60"	[$X360]
		"tall"					"100"
	}

	CHudVote
	{
		"fieldName"		"CHudVote"
		"xpos"			"0"
		"ypos"			"0"
		"wide"			"640"
		"tall"			"480"
		"visible"		"1"
		"enabled"		"1"
		"bgcolor_override"	"0 0 0 0"
		"PaintBackgroundType"	"0" // rounded corners
	}

	// Retained DoD panels for future HUD implementation.
	HudPlayerStatusPanel
	{
		"fieldName"				"HudPlayerStatusPanel"
		"xpos"					"0"
		"ypos"					"290"
		"wide"					"640"
		"tall"					"190"
		"visible"				"1"
		"enabled"				"1"
	}
	HudCapturePanel
	{
		"fieldName"				"HudCapturePanel"
		"xpos"					"c-75"
		"ypos"					"c80"
		"wide"					"150"
		"tall"					"90"
		"visible"				"1"
		"enabled"				"1"
		"icon_space"			"2"
	}
	HudObjectiveIcons
	{
		"fieldName"				"HudObjectiveIcons"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"600"
		"tall"					"200"
		"visible"				"1"
		"enabled"				"1"
		"iconsize"				"24"
		"icon_start_x"			"15"
		"icon_start_y"			"15"
		"separator_width"		"5"		// distance between the icons (including their backgrounds)
		"CornerCutSize"			"5"
		"BackgroundOverlap"		"4"		// amount the background overlaps the icons (width and height)

		"TimerBG"				"HudPanelObjectivesTimer"
		"TimerBG_Flash"			"HudPanelObjectivesTimer_Flash"

		"TimerFont"				"DefaultLarge"

		"time_added_height"		"12"

	}
	overview
	{
		"fieldname"				"overview"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"480"
		"wide"					"0"
		"tall"					"0"
	}
	HudDODCrosshair
	{
		"fieldName"				"HudDODCrosshair"
		"visible"				"1"
		"enabled"				"1"
		"wide"					"640"
		"tall"					"480"
	}
	HudSpecCrosshair
	{
		"fieldName"				"HudSpecCrosshair"
		"visible"				"1"
		"enabled"				"1"
		"wide"					"640"
		"tall"					"480"
	}
	CVProfPanel
	{
		"fieldName"				"CVProfPanel"
		"visible"				"1"
		"enabled"				"1"
		"wide"					"640"
		"tall"					"480"
	}
	CBudgetPanel
	{
		"fieldName"				"CBudgetPanel"
		"visible" 				"1"
		"enabled"				"1"
		"wide"	 				"640"
		"tall"	 				"480"
	}
	CTextureBudgetPanel
	{
		"fieldName"				"CTextureBudgetPanel"
		"visible" 				"1"
		"enabled" 				"1"
		"wide"	 				"640"
		"tall"					"480"
	}
	HudScope
	{
		"fieldName"				"HudZoom"
		"visible" 				"1"
		"enabled" 				"1"
		"wide"	 				"640"
		"tall"	 				"480"
	}
	RoundState
	{
		"fieldName"				"RoundState"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"10"
		"ypos"					"c-30"
		"wide"					"180"
		"tall"					"60"
		"text_xpos"				"8"
		"text_ypos"				"8"
	}
	WarmupLabel
	{
		"fieldName"				"WarmupLabel"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"10"
		"ypos"					"c-30"
		"wide"					"200"
		"tall"					"60"
		"text_xpos"				"8"
		"text_ypos"				"8"
	}
	RestartRoundLabel
	{
		"fieldName"				"RestartRoundLabel"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"10"
		"ypos"					"c0"
		"wide"					"200"
		"tall"					"60"
		"text_xpos"				"8"
		"text_ypos"				"8"
	}
	ReadyRestartLabel
	{
		"fieldName"				"ReadyRestartLabel"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"10"
		"ypos"					"c30"
		"wide"					"400"
		"tall"					"60"
		"text_xpos"				"8"
		"text_ypos"				"8"
	}
	WinPanel_Allies
	{
		"fieldName"				"WinPanel_Allies"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"c-150"
		"ypos"					"255"
		"wide"					"300"
		"tall"					"155"
		"PaintBackgroundType"	"2"

		"icon_ypos"				"4"
		"icon_xpos_left"		"4"
		"icon_xpos_right"		"264"
		"icon_w"				"32"
		"icon_h"				"32"

		"icon_stat_size"			"12"
		"icon_left_stat_x"			"90"
		"icon_left_stat_y1"			"96"
		"icon_left_stat_y2"			"107"
		"icon_left_stat_y3"			"118"
		"icon_right_stat_x"			"230"
		"icon_right_stat_y1"		"96"
		"icon_right_stat_y2"		"107"
		"icon_right_stat_y3"		"118"
	}
	WinPanel_Axis
	{
		"fieldName"				"WinPanel_Axis"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"c-150"
		"ypos"					"255"
		"wide"					"300"
		"tall"					"155"
		"PaintBackgroundType"	"2"

		"icon_ypos"				"4"
		"icon_xpos_left"		"4"
		"icon_xpos_right"		"264"
		"icon_w"				"32"
		"icon_h"				"32"

		"icon_stat_size"			"12"
		"icon_left_stat_x"			"90"
		"icon_left_stat_y1"			"96"
		"icon_left_stat_y2"			"107"
		"icon_left_stat_y3"			"118"
		"icon_right_stat_x"			"230"
		"icon_right_stat_y1"		"96"
		"icon_right_stat_y2"		"107"
		"icon_right_stat_y3"		"118"
	}
	HudTNTPickupPanel
	{
		"fieldName"				"HudTNTPickupPanel"
		"xpos"					"0"
		"ypos"					"c80"
		"wide"					"640"
		"tall"					"70"
		"visible"				"1"
		"enabled"				"1"
	}
	FreezePanel
	{
		"fieldName"				"FreezePanel"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"f0"
		"tall"					"480"
	}
	FreezePanelCallout
	{
		"fieldName"				"FreezePanelCallout"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"200"
		"ypos"					"200"
		"wide"					"100"
		"tall"					"50"
	}
}

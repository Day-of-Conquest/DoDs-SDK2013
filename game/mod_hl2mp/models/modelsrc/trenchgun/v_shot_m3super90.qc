// Created by Crowbar 0.75

$modelname "weapons\v_shot_m3super90.mdl"

$bodygroup "hands"
{
	studio "hands_reference.smd"
}
$bodygroup "studio"
{
	studio "m3_reference.smd"
}


$surfaceprop "default"

$contents "solid"

$illumposition 3.706 -6.714 -8.115

$cdmaterials "models\weapons\V_models\shot_m3super90\"
$cdmaterials "models\weapons\v_hand\"

$attachment "1" "v_weapon.M3_PARENT" 0 3.9 23.4 rotate 0 0 0
$attachment "2" "v_weapon.M3_PARENT" -0.65 4.55 6.5 rotate -35 -180 0
$attachment "muzzle" "v_weapon.M3_PARENT" 0 5.85 30.55 rotate -90 0 0

$cbox 0 0 0 0 0 0

$bbox -12.095 -2.365 -12.834 25.523 9.777 -3.396

$definebone "v_weapon" "" 0 0 0 0 0 89.999982 0 0 0 0 0 0
$definebone "v_weapon.Hands_parent" "v_weapon" -0.073719 0 -4.023378 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.Root34" "v_weapon.Hands_parent" 6.064984 1.693022 10.302076 32.40017 -179.999827 132.693227 0 0 0 0 0 0
$definebone "v_weapon.Left_Arm" "v_weapon.Root34" 0.000001 -0.000001 0.000001 30.055875 114.530941 70.501083 0 0 0 0 0 0
$definebone "v_weapon.eff16" "v_weapon.Left_Arm" 11.132269 0.000023 -0.00002 0.000174 0.001319 0.000176 0 0 0 0 0 0
$definebone "v_weapon.Root16" "v_weapon.eff16" -0.000006 0 0.000001 -25.3665 76.567507 -154.978795 0 0 0 0 0 0
$definebone "v_weapon.Left_Hand" "v_weapon.Root16" 0 -0.000002 0 -5.731471 82.771142 10.431208 0 0 0 0 0 0
$definebone "v_weapon.eff2" "v_weapon.Left_Hand" 3.934568 0.000002 0.000004 -0.000175 0 -0.000119 0 0 0 0 0 0
$definebone "v_weapon.Root17" "v_weapon.eff2" -3.45454 -0.475226 -0.422187 10.48335 -3.580642 -176.078998 0 0 0 0 0 0
$definebone "v_weapon.Left_Thumb01" "v_weapon.Root17" 0.000002 0 0 -13.968654 41.510215 -0.571586 0 0 0 0 0 0
$definebone "v_weapon.Left_Thumb_02" "v_weapon.Left_Thumb01" 2.453374 0.000003 0 -0.000057 -20.57887 -0.000062 0 0 0 0 0 0
$definebone "v_weapon.Left_Thumb03" "v_weapon.Left_Thumb_02" 1.302441 0.00001 -0.000001 -6.633195 -11.22774 -0.842654 0 0 0 0 0 0
$definebone "v_weapon.Root18" "v_weapon.eff2" -0.587191 1.578133 -0.075426 8.664265 1.085871 -82.21944 0 0 0 0 0 0
$definebone "v_weapon.Left_Pinky01" "v_weapon.Root18" -0.000002 -0.000001 0 -3.037769 22.391488 -0.337125 0 0 0 0 0 0
$definebone "v_weapon.Left_Pinky02" "v_weapon.Left_Pinky01" 1.253014 -0.00001 -0.000001 -0.520937 -0.659181 -0.134356 0 0 0 0 0 0
$definebone "v_weapon.Left_Pinky03" "v_weapon.Left_Pinky02" 0.821604 0.000002 0 0.144783 15.670698 0.029337 0 0 0 0 0 0
$definebone "v_weapon.Root19" "v_weapon.eff2" -0.164639 0.806246 -0.051105 9.520494 -0.658271 -94.484869 0 0 0 0 0 0
$definebone "v_weapon.Left_Ring01" "v_weapon.Root19" 0 -0.000001 0 -1.45308 13.489033 -0.642969 0 0 0 0 0 0
$definebone "v_weapon.Left_Ring02" "v_weapon.Left_Ring01" 1.570704 0.000001 0 0.048068 14.03839 0.015416 0 0 0 0 0 0
$definebone "v_weapon.Left_Ring03" "v_weapon.Left_Ring02" 0.991982 -0.000005 -0.000001 0.059355 3.325052 0.008082 0 0 0 0 0 0
$definebone "v_weapon.Root20" "v_weapon.eff2" 0 0 0.000001 9.200383 -4.284865 -96.936094 0 0 0 0 0 0
$definebone "v_weapon.Left_Middle01" "v_weapon.Root20" -0.000004 0 0 -3.758833 3.465936 -0.000166 0 0 0 0 0 0
$definebone "v_weapon.Left_Middle02" "v_weapon.Left_Middle01" 1.625753 -0.000009 -0.000002 0.049614 27.346302 0.031459 0 0 0 0 0 0
$definebone "v_weapon.Left_Middle03" "v_weapon.Left_Middle02" 1.212423 0.000008 0 0.136647 -10.038851 0.003958 0 0 0 0 0 0
$definebone "v_weapon.Root21" "v_weapon.eff2" -0.121887 -0.855046 0.081739 9.035256 -5.481201 -97.742883 0 0 0 0 0 0
$definebone "v_weapon.Left_Index01" "v_weapon.Root21" -0.000002 0 0 -1.815073 5.237806 -0.000111 0 0 0 0 0 0
$definebone "v_weapon.Left_Index02" "v_weapon.Left_Index01" 1.66527 -0.000003 0.000001 -0.023894 16.849315 -0.010194 0 0 0 0 0 0
$definebone "v_weapon.Left_Index03" "v_weapon.Left_Index02" 0.974182 0.000005 -0.000001 -0.027846 -6.786116 0.000119 0 0 0 0 0 0
$definebone "v_weapon.Root36" "v_weapon.Hands_parent" -6.050437 -1.839863 8.972858 32.399941 179.999896 132.692708 0 0 0 0 0 0
$definebone "v_weapon.Right_Arm" "v_weapon.Root36" 0 0.000001 0 35.525313 131.193222 179.999773 0 0 0 0 0 0
$definebone "v_weapon.eff18" "v_weapon.Right_Arm" 11.106726 -0.000021 -0.000007 -0.000001 -0.00132 0 0 0 0 0 0 0
$definebone "v_weapon.Root23" "v_weapon.eff18" -0.000002 -0.000001 -0.000001 2.205314 -2.905816 -62.617754 0 0 0 0 0 0
$definebone "v_weapon.Right_Hand" "v_weapon.Root23" -0.000003 0 0 -11.967368 2.623113 -0.044065 0 0 0 0 0 0
$definebone "v_weapon.eff9" "v_weapon.Right_Hand" 3.917583 0.000002 -0.000009 0.000003 0.000169 -0.000004 0 0 0 0 0 0
$definebone "v_weapon.Root24" "v_weapon.eff9" -3.456715 0.410929 -0.408732 15.069708 -2.721952 -176.67067 0 0 0 0 0 0
$definebone "v_weapon.Right_Thumb01" "v_weapon.Root24" -0.000001 0 0.000001 -17.439524 -45.141113 0.272209 0 0 0 0 0 0
$definebone "v_weapon.Right_Thumb02" "v_weapon.Right_Thumb01" 2.572062 0.000084 0.000081 0.000169 28.233753 -0.000979 0 0 0 0 0 0
$definebone "v_weapon.Right_Thumb03" "v_weapon.Right_Thumb02" 1.277195 0.000014 0.000038 0.000114 8.835411 -0.000177 0 0 0 0 0 0
$definebone "v_weapon.Root25" "v_weapon.eff9" -0.116032 0.857849 0.087382 15.362777 -1.934135 94.972565 0 0 0 0 0 0
$definebone "v_weapon.Right_Index01" "v_weapon.Root25" 0.000002 0.000001 0 -0.051052 -2.615209 0.116545 0 0 0 0 0 0
$definebone "v_weapon.Right_Index02" "v_weapon.Right_Index01" 1.680861 -0.000014 0 -0.000057 -20.396033 0.000005 0 0 0 0 0 0
$definebone "v_weapon.Right_Index03" "v_weapon.Right_Index02" 0.978208 -0.00001 0 -0.000001 14.588191 0.000005 0 0 0 0 0 0
$definebone "v_weapon.Root26" "v_weapon.eff9" 0 0 -0.000001 15.43938 -2.840382 95.790289 0 0 0 0 0 0
$definebone "v_weapon.Right_Middle01" "v_weapon.Root26" 0 0.000001 0 -1.540856 -7.069954 -1.881589 0 0 0 0 0 0
$definebone "v_weapon.Right_Middle02" "v_weapon.Right_Middle01" 1.639589 0.000007 0.000001 0 -13.186968 0.000005 0 0 0 0 0 0
$definebone "v_weapon.Right_Middle03" "v_weapon.Right_Middle02" 1.226206 0.000004 0.000001 0 -2.397484 0.000062 0 0 0 0 0 0
$definebone "v_weapon.Root27" "v_weapon.eff9" -0.141087 -0.790348 -0.057467 15.229621 -1.019294 97.116452 0 0 0 0 0 0
$definebone "v_weapon.Right_Ring01" "v_weapon.Root27" -0.000002 -0.000001 0 -5.653776 -14.49744 -0.973566 0 0 0 0 0 0
$definebone "v_weapon.Right_Ring02" "v_weapon.Right_Ring01" 1.570883 0.000003 -0.000002 0.000001 -8.897634 0.000062 0 0 0 0 0 0
$definebone "v_weapon.Right_Ring03" "v_weapon.Right_Ring02" 0.964522 -0.000011 -0.000001 0.000001 -4.888706 -0.00011 0 0 0 0 0 0
$definebone "v_weapon.Root28" "v_weapon.eff9" -0.623751 -1.549207 -0.080387 15.356075 1.850652 91.51683 0 0 0 0 0 0
$definebone "v_weapon.Right_Pinky01" "v_weapon.Root28" 0 0 -0.000001 -11.50723 -14.798875 4.203796 0 0 0 0 0 0
$definebone "v_weapon.Right_Pinky02" "v_weapon.Right_Pinky01" 1.295092 0 0.000001 -0.000001 -10.518993 0.000063 0 0 0 0 0 0
$definebone "v_weapon.Right_Pinky03" "v_weapon.Right_Pinky02" 0.799097 -0.000002 0.000001 -0.000173 -2.108431 -0.000109 0 0 0 0 0 0
$definebone "v_weapon.M3_PARENT" "v_weapon" 3.542127 3.12732 21.392694 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.M3_SHELL" "v_weapon.M3_PARENT" 0.006072 -2.533671 -10.5511 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.M3_TRIGGER" "v_weapon.M3_PARENT" 0 2.96454 1.156452 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.M3_LOADER" "v_weapon.M3_PARENT" 0 2.859376 3.645414 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.M3_CHAMBER" "v_weapon.M3_PARENT" 0 4.156439 3.820694 0 0 0 0 0 0 0 0 0
$definebone "v_weapon.M3_PUMP" "v_weapon.M3_PARENT" -0.000001 3.192405 10.568916 0 0 0 0 0 0 0 0 0


$sequence "idle" {
	"v_shot_m3super90_anims\idle.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 12
}

$sequence "shoot1" {
	"v_shot_m3super90_anims\shoot1.smd"
	activity "ACT_VM_PRIMARYATTACK" 1
	{ event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
	{ event 6001 0 "2" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 32
}

$sequence "shoot2" {
	"v_shot_m3super90_anims\shoot2.smd"
	activity "ACT_VM_PRIMARYATTACK" 1
	{ event AE_MUZZLEFLASH 0 "SHOTGUN MUZZLE" }
	{ event 6001 0 "2" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 37
}

$sequence "insert" {
	"v_shot_m3super90_anims\insert.smd"
	activity "ACT_VM_RELOAD" 1
	{ event 5004 0 "Weapon_M3.Insertshell" }
	fadein 0.2
	fadeout 0.2
	fps 55
}

$sequence "after_reload" {
	"v_shot_m3super90_anims\after_reload.smd"
	activity "ACT_SHOTGUN_RELOAD_FINISH" 1
	{ event 5004 12 "Weapon_M3.Pump" }
	fadein 0.2
	fadeout 0.2
	fps 40
}

$sequence "start_reload" {
	"v_shot_m3super90_anims\start_reload.smd"
	activity "ACT_SHOTGUN_RELOAD_START" 1
	fadein 0.2
	fadeout 0.2
	fps 40
}

$sequence "draw" {
	"v_shot_m3super90_anims\draw.smd"
	activity "ACT_VM_DRAW" 1
	{ event 5004 11 "Weapon_M3.Pump" }
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "pump_layer" {
	"v_shot_m3super90_anims\pump_layer.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 37
}

$sequence "pump" {
	"v_shot_m3super90_anims\pump.smd"
	activity "ACT_SHOTGUN_PUMP" 1
	{ event 5004 8 "Weapon_M3.Pump" }
	fadein 0.2
	fadeout 0.2
	blendlayer "pump_layer" 0 5 25 30
	snap
	fps 37
}

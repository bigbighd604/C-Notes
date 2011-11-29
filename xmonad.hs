import XMonad
import XMonad.Util.EZConfig
import XMonad.Util.Run
import XMonad.Config.Gnome

main = xmonad $ gnomeConfig
  { terminal = "gnome-terminal"
  }
  `additionalKeys`
  [ ((controlMask .|. shiftMask, xK_z), spawn "gnome-screensaver-command --lock")
  , ((controlMask, xK_Print), spawn "sleep 0.2; scrot -s")
  , ((0, xK_Print), spawn "scrot")
  ]

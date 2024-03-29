#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const int newclientathead = 0;
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 8;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 8;       /* vert inner gap between windows */
static const unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 8;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "SauceCodePro Nerd Font Mono:size=16" };
static const char dmenufont[]       = "SauceCodePro Nerd Font Mono:size=16";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]        = "#42A5F5";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
//static const char *tags[] = { "\uf120", "\uf7ae", "\uf121", "\uf04b", "\ue62e", "\uf251", "\ue727", "\uf537", "\uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Android Emulator", NULL,       NULL,       0,            1,           -1 },
	{ "Emulator", NULL,       NULL,       0,            1,           -1 },
	{ "qemu-system-i386", NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ "Grid",      grid },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *thunarcmd[]  = { "thunar", NULL };
static const char *baiducmd[]  = { "baidunetdisk", NULL };
static const char *chromecmd[]  = { "google-chrome-stable", "--password-store=basic", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
static const char *edgecmd[]  = { "microsoft-edge-stable", NULL };
static const char *torcmd[]  = { "tor-browser", NULL };

static const char *vscodecmd[]  = { "code", NULL };
static const char *pycharmcmd[]  = { "pycharm", NULL };
static const char *ideacmd[]  = { "idea", NULL };
static const char *markdowncmd[]  = { "marktext", "-n", NULL };
static const char *textcmd[]  = { "nvim-qt", NULL };
static const char *geometrycmd[]  = { "geogebra", NULL };
static const char *androidstudiocmd[]  = { "android-studio", NULL };
static const char *qtcmd[]  = { "qtcreator", NULL };
static const char *arduinocmd[]  = { "arduino", NULL };
static const char *xmindcmd[]  = { "xmind", NULL };
static const char *qemucmd[]  = { "virt-manager", NULL };
static const char *vlccmd[]  = { "vlc", NULL };
static const char *musiccmd[]  = { "netease-cloud-music", NULL };
static const char *obscmd[]  = { "obs", NULL };
static const char *remminacmd[]  = { "remmina", NULL };
static const char *steamcmd[]  = { "steam-native", NULL };
static const char *lutriscmd[]  = { "lutris", NULL };

static const char *upvol[]   = { "volumeControl.sh", "up", NULL };
static const char *downvol[] = { "volumeControl.sh", "down",  NULL };
static const char *uplight[]   = { "brightnessControl.sh", "up", NULL };
static const char *downlight[] = { "brightnessControl.sh", "down", NULL };
static const char *mutevol[] = { "brightnessControl.sh", "toggle", NULL };

static const char *toggleinfo[] = { "dwm-status.sh",  "new", NULL };

static const char *xdisplay[] = { "xdisplay.sh",  NULL };
static const char *toggleotherdisplay[] = { "toggle-other-display.sh",  NULL };
static const char *toggledisplay[] = { "toggle-display.sh",  NULL };
static const char *togglealldisplay[] = { "toggle-all-display.sh",  NULL };

static const char *wpcmd[]  = { "randomWallpaper.sh", NULL };
static const char *wvcmd[]  = { "randomVideoWallpaper.sh", NULL };
static const char *slockcmd[]  = { "slock", NULL };
static const char *sktogglecmd[]  = { "sck-tog.sh", NULL };
static const char *snapshotcmd[]  = { "snapshot.sh", NULL };
static const char *transtifycmd[]  = { "transtify.sh", NULL };
static const char *scriptmanagercmd[]  = { "scriptsManager.sh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static const char *setcolemakcmd[]  = { "setxmodmap-colemak.sh", NULL };
static const char *setqwertycmd[]  = { "setxmodmap-qwerty.sh", NULL };

static const char *suspendcmd[]  = { "suspend.sh", NULL };

static const char *screenshotcmd[] = { "flameshot", "gui", NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	{ MODKEY,              XK_s,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_w,                    spawn,          {.v = toggleinfo } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY,              XK_p,                    spawn,          {.v = scriptmanagercmd } },
	{ MODKEY|ShiftMask,    XK_Return,               spawn,          {.v = thunarcmd } },
	{ MODKEY|ShiftMask,    XK_b,                    spawn,          {.v = baiducmd } },
	{ MODKEY|ShiftMask,    XK_g,                    spawn,          {.v = chromecmd } },
	{ MODKEY|ShiftMask,    XK_f,                    spawn,          {.v = firefoxcmd } },
	{ MODKEY|ShiftMask,    XK_w,                    spawn,          {.v = edgecmd } },
	{ MODKEY|ShiftMask,    XK_h,                    spawn,          {.v = torcmd } },
	{ MODKEY|ShiftMask,    XK_d,                    spawn,          {.v = geometrycmd } },
	{ MODKEY|ShiftMask,    XK_c,                    spawn,          {.v = vscodecmd } },
	{ MODKEY|ShiftMask,    XK_p,                    spawn,          {.v = pycharmcmd } },
	{ MODKEY|ShiftMask,    XK_i,                    spawn,          {.v = ideacmd } },
	{ MODKEY|ShiftMask,    XK_a,                    spawn,          {.v = androidstudiocmd } },
	{ MODKEY|ShiftMask,    XK_n,                    spawn,          {.v = markdowncmd } },
	{ MODKEY|ShiftMask,    XK_t,                    spawn,          {.v = textcmd } },
	{ MODKEY|ShiftMask,    XK_q,                    spawn,          {.v = qtcmd } },
	{ MODKEY|ShiftMask,    XK_e,                    spawn,          {.v = arduinocmd } },
	{ MODKEY|ShiftMask,    XK_x,                    spawn,          {.v = xmindcmd } },
	{ MODKEY|ShiftMask,    XK_k,                    spawn,          {.v = qemucmd } },
	{ MODKEY|ShiftMask,    XK_v,                    spawn,          {.v = vlccmd } },
	{ MODKEY|ShiftMask,    XK_m,                    spawn,          {.v = musiccmd } },
	{ MODKEY|ShiftMask,    XK_o,                    spawn,          {.v = obscmd } },
	{ MODKEY|ShiftMask,    XK_r,                    spawn,          {.v = remminacmd } },
	{ MODKEY|ShiftMask,    XK_s,                    spawn,          {.v = steamcmd } },
	{ MODKEY|ShiftMask,    XK_l,                    spawn,          {.v = lutriscmd } },
	{ MODKEY|ControlMask,  XK_p,                    spawn,          {.v = suspendcmd } },
	{ MODKEY|ControlMask,  XK_s,                    spawn,          {.v = snapshotcmd } },
	{ MODKEY|ControlMask,  XK_t,                    spawn,          {.v = transtifycmd } },
	{ MODKEY|ControlMask,  XK_backslash,            spawn,          {.v = sktogglecmd } },
	{ MODKEY,              XK_b,                    spawn,          {.v = wpcmd } },
	{ MODKEY|ControlMask,  XK_b,                    spawn,          {.v = wvcmd } },
	{ MODKEY|ControlMask,  XK_e,                    spawn,          {.v = slockcmd } },
	{ 0,                   XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
	{ MODKEY,              XK_backslash,            spawn,          {.v = mutevol } },
	{ MODKEY,              XK_bracketleft,          spawn,          {.v = downvol } },
	{ MODKEY,              XK_bracketright,         spawn,          {.v = upvol   } },
	{ MODKEY|ControlMask,  XK_bracketleft,          spawn,          {.v = downlight } },
	{ MODKEY|ControlMask,  XK_bracketright,         spawn,          {.v = uplight   } },
	{ MODKEY,              XK_minus,                spawn,          {.v = xdisplay   } },
	{ MODKEY|ControlMask,  XK_minus,                spawn,          {.v = toggleotherdisplay   } },
	{ MODKEY,              XK_equal,                spawn,          {.v = toggledisplay } },
	{ MODKEY|ControlMask,  XK_equal,                spawn,          {.v = togglealldisplay } },
	{ 0,                   XK_Print,                spawn,          {.v = screenshotcmd } },
	{ MODKEY|ControlMask,  XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask,  XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY,              XK_n,                    viewtoleft,     {0} },
	{ MODKEY,              XK_m,                    viewtoright,    {0} },
	{ MODKEY|ControlMask,  XK_n,                    tagtoleft,      {0} },
	{ MODKEY|ControlMask,  XK_m,                    tagtoright,     {0} },
	{ MODKEY|ControlMask,  XK_h,                    incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,  XK_l,                    incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_i,                    hidewin,        {0} },
	{ MODKEY|ControlMask,  XK_i,                    restorewin,     {0} },
	{ MODKEY,              XK_o,                    hideotherwins,  {0}},
	{ MODKEY|ControlMask,  XK_o,                    restoreotherwins, {0}},
	{ MODKEY|ControlMask,  XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY,              XK_q,                    killclient,     {0} },
	{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_g,                    setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_v,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_f,                    fullscreen,     {0} },
	{ MODKEY,              XK_space,                setlayout,      {0} },
	{ MODKEY|ControlMask,  XK_space,                togglefloating, {0} },
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ControlMask,  XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ControlMask,  XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,  XK_period,               tagmon,         {.i = +1 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


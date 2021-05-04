/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "ubuntu-mono:size=10" };
static const char dmenufont[]       = "ubuntu-mono:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Zoom",     NULL,       NULL,       1 << 6,       0,           -1 },
	{ "Signal",   NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Caprine",  NULL,       NULL,       1 << 7,       0,           -1 },
	{ "RStudio",  NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Slack",    NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,       NULL,     "mutt",       1 << 8,       0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "terminator", NULL };
static const char *ffxcmd[]   = { "firefox", NULL };
static const char *ffxprivcmd[]   = { "firefox", "--private-window", NULL };
static const char *lockcmd[]  = { "slock", NULL };
static const char *muttcmd[]  = { "terminator", "--title=mutt", "-e", "mutt", NULL };
static const char *cambcmd[]  = { "terminator", "--profile=highdpi", "-e", "sshpass -f ~/.ssh/ssh_pass ssh -X nibr@cambridge.csail.mit.edu", NULL };
static const char *signalcmd[]  = { "signal-desktop", NULL };
static const char *rstudiocmd[]  = { "rstudio", NULL };
static const char *volumeinc[]  = { "/home/nibr/bin/volume-control", "up",   NULL };
static const char *volumedec[]  = { "/home/nibr/bin/volume-control", "down", NULL };
static const char *volumeoff[]  = { "/home/nibr/bin/volume-control", "mute", NULL };
static const char *micoff[]  = { "/home/nibr/bin/volume-control", "micmute", NULL };
static const char *slackcmd[]  = { "/snap/bin/slack", NULL };
static const char *brightnessinc[]  = { "brightnessctl", "-e", "s", "2%+", NULL };
static const char *brightnessmax[]  = { "brightnessctl", "-e", "s", "100%", NULL };
static const char *brightnessdec[]  = { "brightnessctl", "-e", "s", "2%-", NULL };
static const char *brightnessmin[]  = { "brightnessctl", "-e", "s", "1%", NULL };
static const char *poweroff[]  = { "poweroff", NULL };
static const char *autodisplay[] = { "/home/nibr/bin/auto-display", NULL };
static const char *wifitoggle[] = { "/home/nibr/bin/wifi-toggle", NULL };
static const char *nolap[] = { "autorandr", "-c", "nolap", NULL };
static const char *xe242[] = { "autorandr", "-c", "E242", NULL };
static const char *logout[]  = { "bash", "-c", "/usr/bin/pkill -P $( pgrep .xsession )", NULL };
static const char *dunstpause[]  = { "dunstctl", "set-paused", "toggle", NULL};


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.005} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.005} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = ffxcmd} },
	{ ControlMask|ShiftMask,        XK_p,      spawn,          {.v = ffxprivcmd} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = signalcmd} },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd} },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = muttcmd} },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = cambcmd} },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          {.v = nolap} },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = xe242} },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = dunstpause} },

	{ 0,                XF86XK_AudioMute,      spawn,          {.v = volumeoff} },
	{ Mod4Mask,                    XK_F1,      spawn,          {.v = volumeoff} },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = volumedec} },
	{ Mod4Mask,                    XK_F2,      spawn,          {.v = volumedec} },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = volumeinc} },
	{ Mod4Mask,                    XK_F3,      spawn,          {.v = volumeinc} },
	{ 0,             XF86XK_AudioMicMute,      spawn,          {.v = micoff} },
	{ Mod4Mask,                    XK_F4,      spawn,          {.v = micoff} },

	{ 0,        XF86XK_MonBrightnessDown,      spawn,          {.v = brightnessdec} },
	{ ShiftMask,XF86XK_MonBrightnessDown,      spawn,          {.v = brightnessmin} },
	{ Mod4Mask,                    XK_F5,      spawn,          {.v = brightnessdec} },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = brightnessinc} },
	{ ShiftMask,  XF86XK_MonBrightnessUp,      spawn,          {.v = brightnessmax} },
	{ Mod4Mask,                    XK_F6,      spawn,          {.v = brightnessinc} },
	{ 0,                  XF86XK_Display,      spawn,          {.v = autodisplay} },
	{ Mod4Mask,                    XK_F7,      spawn,          {.v = autodisplay} },

	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = rstudiocmd} },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = slackcmd} },
	{ MODKEY|ShiftMask,        XK_Delete,      spawn,          {.v = poweroff} },
	{ MODKEY|ShiftMask,     XK_BackSpace,      spawn,          {.v = logout} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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


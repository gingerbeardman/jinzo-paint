#ifndef	UNICODE
#define	UNICODE
#endif	/* UNICODE */
#ifndef	_UNICODE
#define	_UNICODE
#endif	/* _UNICODE */
#define	STRICT
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include	<commdlg.h>
#include <wingdi.h> 
#include <Winuser.h>
#include "resource.h" 
#include <Winbase.h>
#include "jzpaint.h"
DWORD	gle=0;

/* =====================================================================
	関数のプロトタイプ宣言
===================================================================== */
// WinMainから呼ばれる関数
BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

// コールバック関数
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

// ダイアログ関数
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK TextDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK bgcDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK colDlgProc(HWND, UINT, WPARAM, LPARAM);

// メッセージ処理関数
void OnCreate(HWND, WPARAM, LPARAM);
void OnDestroy(HWND, WPARAM, LPARAM);
void OnPaint(HWND, WPARAM, LPARAM);
void OnHelp(HWND, WPARAM, LPARAM);
void mouse_down(HWND,WPARAM, LPARAM);
void mouse_move(HWND,WPARAM, LPARAM);
void mouse_up(HWND,WPARAM, LPARAM);
void do_undo(HWND);
void undo_store(HWND);
BOOL save_bmp(HWND);
void force_save(void);
void backup_save(void);
BOOL load_bmp(HWND);
void redraw(HWND);
void paste(HWND);
void paste_cansel(HWND);
void cut_region(HWND);
void line_start(HWND);
void line_end(HWND);
void select_region(HWND);
void select_free(HWND);
void select_all(HWND);
void select_magicwand(HWND);
void pen_button(HWND);
BOOL col_button(HWND,int);
void pat_button(HWND,int);
void erase_button(HWND hWnd);
void mag_button(HWND, int);
void paint(HWND, int, int);
void free_paste_start(void);
void input_text(HWND);
void background_color(HWND);
void select_all(HWND);
void clipboard_inverse(HWND);
void clipboard_lasso(HWND);
void clipboard_magicwand(HWND);
void clipboard_bold(HWND);
void clipboard_crop(BYTE *,BYTE);
void magicwand(int,int);

int flag=0,xflag,yflag;
int	cbh;

BOOL	fill_flag=FALSE;

REGION_STATE region_flag=PEN;

int		region_sx,region_sy;
int		region_ex,region_ey;
int		select_x,select_y;
int		select_dx,select_dy;
int		select_free_x,select_free_y;
DWORD	ticks;
BOOL	cut_flag=FALSE,rect_flag=TRUE;
typedef enum {
	AREA_RECT,AREA_FREE,AREA_ALL,AREA_MAGICWAND
} AREA;
AREA start_select_rect_flag=AREA_RECT;
int		drag_tm=-1;
int		drag_x,drag_y,drag_dx,drag_dy;
int		drag_tap_flag=-1;
int		tm=0,line_tm=-1;
//int		backup_flag=0;
BOOL	clipboard_flag=FALSE;

typedef enum {
	UP,MOVE,DOWN
} MOUSE_TAP;
MOUSE_TAP mouse_flag=UP;


typedef enum {
	TOOLPIC_PEN,TOOLPIC_ERASE,TOOLPIC_SPUIT,TOOLPIC_HAND,TOOLPIC_PAINT,TOOLPIC_FONT,
	TOOLPIC_LINE,TOOLPIC_RECT,TOOLPIC_RECTFILL,TOOLPIC_CIRCLE,TOOLPIC_CIRCLEFILL,
	TOOLPIC_COPY_RECT,TOOLPIC_COPY_FREE,TOOLPIC_COPY_MAGICWAND
} TOOLPIC;
TOOLPIC toolpic;

int		hand_x,hand_y,hand_dx,hand_dy,hand_lx,hand_ly;

// ファイル・メニュー関数
void OnFileExit(HWND);			// 終了

// ヘルプ・メニュー関数
void OnHelpAbout(HWND);			// バージョン情報
void OnColorOption(HWND);

/* =====================================================================
	グローバル変数
===================================================================== */
TCHAR gszAppName[]	= TEXT("JZPaint16");	// クラスの名前
TCHAR gszAppTitle[]	= TEXT("JZPaint16");	// ウィンドウ・タイトル

HINSTANCE ghInst	= NULL;		// 現在のインスタンス
HWND      ghWnd		= NULL;		// メインウィンドウのハンドル
HWND      ghWndCB	= NULL;		// コマンドバーのハンドル

HBITMAP	bp_mesh;

HDC		baseDC	= NULL;		// オフスクリーン
HBITMAP	baseBmp	= NULL;
HDC		base24DC	= NULL;		// オフスクリーン
HBITMAP	base24Bmp	= NULL;
HDC		tmpDC	= NULL;		// オフスクリーン
HBITMAP	tmpBmp	= NULL;
HDC		tmp24DC	= NULL;		// オフスクリーン
HBITMAP	tmp24Bmp	= NULL;
HDC		dialogDC	= NULL;		// オフスクリーン
HBITMAP	dialogBmp	= NULL;
HDC		colbuttonDC	= NULL;		// オフスクリーン
HBITMAP	colbuttonBmp = NULL;

BOOL	pspc_flag;

BYTE *baseBuf,*base24Buf,*dialogBuf,*colbuttonBuf,*undoBuf,*tmpBuf,*tmp24Buf,*copyBuf,*freeareaBuf;

int	winWidth,winHeight;  
int picWidth,picHeight;

POINT	pts[3];
COLORREF col[4];

int	pcolor=0;
int	pstyle=0;
int	ppat=0; 
int	perase=0;
int	bgc=16;

int	pat_inv=0;

BOOL	color_mode[16][16],color_mode_tmp[16][16];
int color_check_id[4][4]={
	{IDC_CHECK0_0,IDC_CHECK0_1,IDC_CHECK0_2,IDC_CHECK0_3},
	{IDC_CHECK1_0,IDC_CHECK1_1,IDC_CHECK1_2,IDC_CHECK1_3}, 
	{IDC_CHECK2_0,IDC_CHECK2_1,IDC_CHECK2_2,IDC_CHECK2_3},
	{IDC_CHECK3_0,IDC_CHECK3_1,IDC_CHECK3_2,IDC_CHECK3_3}
};

BOOL	conv_mode[16][16],conv_mode_tmp[16][16];
int conv_check_id[4][4]={
	{IDC_CCONV_B_B ,IDC_CCONV_B_DG, IDC_CCONV_B_LG, IDC_CCONV_B_W },
	{IDC_CCONV_DG_B,IDC_CCONV_DG_DG,IDC_CCONV_DG_LG,IDC_CCONV_DG_W},
	{IDC_CCONV_LG_B,IDC_CCONV_LG_DG,IDC_CCONV_LG_LG,IDC_CCONV_LG_W},
	{IDC_CCONV_W_B ,IDC_CCONV_W_DG, IDC_CCONV_W_LG, IDC_CCONV_W_W }
};
BYTE	cconv[16],cconv_tmp[16];
BOOL	conv_flag=FALSE;

// ルーペ
int		lmag[]={1,2,3,4,8};
int		loope_num=0;
int		loope_mag=lmag[loope_num];
int		loope_x=0,loope_y=0;
int		loope_x0=0,loope_y0=0;
int		loope_move=8;
BOOL	loope_orginal_size_view_flag[5];

BOOL	erase_put_flag;
int		erase_x,erase_y;

int		font_x,font_y;

BOOL	action_button_flag=FALSE;

BOOL	overwrite_flag=FALSE;
BOOL	clipboard_save_flag=FALSE;

BYTE rotate_table[8][4]={
	{1,3,6,4},
	{2,0,7,5},
	{3,1,4,6},
	{0,2,5,7},
	{7,5,2,0},
	{4,6,3,1},
	{5,7,0,2},
	{6,4,1,3}
};	
int	rotate_flag=0;

static TBBUTTON tb[] = {
//	{ 0, 0,            TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0, 0, -1},
	{ BM_TOOL, IDM_TOOLBUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
	{ 0, 0,            TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0, 0, -1},
	{ BM_PEN_SEL, IDM_PENBUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
	{ BM_COLOR, IDM_COLBUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
	{ BM_PATTERN, IDM_PATBUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
	{ 0, 0,            TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0, 0, -1},
	{ BM_ERASE_USEL, IDM_ERABUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
//	{ 0, 0,            TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0, 0, -1},
	{ BM_LOOPE, IDM_MAGBUTTON,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
//	{ 0, 0,            TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0, 0, -1},
	{ BM_ETC, IDM_UNDO     ,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, -1},
};
HMENU	pen_menu,col_menu,col_p_menu,pat_menu,erase_menu,line_menu,tool_menu;

BOOL	test_flag=FALSE;
int		test_n=0;
BOOL	col_menu_flag=FALSE;
BOOL	pat_menu_flag=FALSE;
BOOL	loope_menu_flag=FALSE;
BOOL	tool_menu_flag=FALSE;
			
typedef enum {
	COLOR_LOAD_START,COLOR_LOAD_END,COLOR_LOAD_NONE
} COLOR_LOAD_STRUCT;
COLOR_LOAD_STRUCT color_load_mode=COLOR_LOAD_NONE;

typedef enum {
	COLOR_MODE_START,COLOR_MODE_END,COLOR_MODE_NONE
} COLOR_MODE_STRUCT;
COLOR_MODE_STRUCT color_mask_mode=COLOR_MODE_NONE;

typedef enum {
	COLMODE_NONE,COLMODE_MOVE,COLMODE_MOVE2
} COLMODE_STATE;
COLMODE_STATE colmode_state=COLMODE_NONE;

typedef enum {
	COLOR_CONV_START,COLOR_CONV_END,COLOR_CONV_NONE
} COLOR_CONV_STRUCT;
COLOR_CONV_STRUCT color_conv_mode=COLOR_CONV_NONE;

typedef enum {
	COLCONV_NONE,COLCONV_MOVE,COLCONV_MOVE2
} COLCONV_STATE;
COLCONV_STATE colconv_state=COLCONV_NONE;

typedef enum {
	COLOR_SET_START,COLOR_SET_END,COLOR_SET_NONE
} COLOR_SET_STRUCT;
COLOR_SET_STRUCT color_set_mode=COLOR_SET_NONE;

typedef enum {
	COLSET_NONE,COLSET_MOVE,COLSET_MOVE2,COLSET_RED,COLSET_BLUE,COLSET_GREEN
} COLSET_STATE;
COLSET_STATE colset_state=COLSET_NONE;

typedef enum {
	COLLOAD_NONE,COLLOAD_MOVE,COLLOAD_MOVE2,COLLOAD_RED,COLLOAD_BLUE,COLLOAD_GREEN
} COLLOAD_STATE;
COLLOAD_STATE colload_state=COLLOAD_NONE;

typedef enum {
	COLOR_BG_START,COLOR_BG_END,COLOR_BG_NONE
} COLOR_BG_STRUCT;
COLOR_BG_STRUCT color_bg_mode=COLOR_BG_NONE;

typedef enum {
	COLBG_NONE,COLBG_MOVE,COLBG_MOVE2
} COLBG_STATE;
COLBG_STATE colbg_state=COLBG_NONE;

typedef enum {
	COLOR_AREA_START,COLOR_AREA_END,COLOR_AREA_NONE
} COLOR_AREA_STATE;
COLOR_AREA_STATE color_area_mode=COLOR_AREA_NONE;

typedef enum {
	COLAREA_NONE,COLAREA_MOVE,COLAREA_MOVE2
} COLAREA_STATE;
COLAREA_STATE colarea_state=COLAREA_NONE;

BYTE	color_row=0,color_column=0;
BOOL	color_load_open_flag;

BYTE	init_color[16+5+16][3] = {
	{  0,  0,  0},
	{128,128,128},
	{192,192,192},
	{255,255,255},
	{128,  0,  0},
	{  0,  0,128},
	{128,  0,128},
	{  0,128,  0},
	{128,128,  0},
	{  0,128,128},
	{255,  0,  0},
	{  0,  0,255},
	{255,  0,255},
	{  0,255,  0},
	{255,255,  0},
	{  0,255,255},

	{  0,  0,  0},
	{128,128,128},
	{192,192,192},
	{255,255,255},
	{  0,  0,  0},

	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
	{  0,  0,  0},
};
BYTE	tmp_color[16][3];

BOOL	area_color[16],tmp_area_color[16];

int	col_pulldown_x_pos;
BOOL colsel_pflag,col_press_flag;
int	colbutton_x,colbutton_y;
int	tmp_col_sel=0;
int	tmp_bgc_color;
int col_counter=-1;

int		colset_x=0,colset_y=0;
int		colset_move_x,colset_move_y;
int		colset_move_sx,colset_move_sy;
int		colset_exchange=-1;

BOOL	put_colbutton_flag=TRUE;
BOOL	active_flag=TRUE;

BOOL	called_jzv_flag;

int		line_adjust=0;
int		line_adjust_max=0;

/* =====================================================================
	Windowsプログラムのメイン関数
===================================================================== */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPTSTR lpszCmdParm, int nCmdShow)
{
	MSG    msg;

    // ウィンドウの検索
    HWND hWnd = FindWindow(gszAppName, gszAppTitle);
    if (hWnd) {
        // 既に起動しているウィンドウを最前面に移動して終了
        SetForegroundWindow(hWnd);
        return 0;
    }

	ghInst = hInstance;

	if (!hPrevInstance) {					// ほかのインスタンス実行中 ?
		if (!InitApplication(hInstance))	// 共通の初期化処理
			return FALSE;					// 初期化に失敗した場合は終了
	}
	if (!InitInstance(hInstance, nCmdShow))	// インスタンス毎の初期化処理
		return FALSE;						// 失敗した場合は終了

	while (GetMessage(&msg, NULL, 0, 0)) {	// メッセージの取得とディスパッチ
		TranslateMessage(&msg);				// 仮想キーコードの変換
		DispatchMessage(&msg);				// メッセージのディスパッチ
	}
	return msg.wParam;		// PostQuitMessage()関数の戻り値を返す
}

/* =====================================================================
	ウィンドウ・クラスの登録
===================================================================== */
BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style		 = 0;					// クラス・スタイル
	wc.lpfnWndProc	 = MainWndProc;			// ウィンドウ・プロシージャ
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;			// インスタンス・ハンドル
	wc.hIcon		 = NULL;
	wc.hCursor		 = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	 = NULL;				// メニューの名前
	wc.lpszClassName = gszAppName;			// ウィンドウ・クラスの名前

	return RegisterClass(&wc);	// ウィンドウ・クラスの登録
}

/* =====================================================================
	ウィンドウの作成と表示
===================================================================== */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	ghWnd = CreateWindow( 
 		gszAppName,				// 登録されたウィンドウ・クラスの名前
		gszAppTitle,			// タイトル・バーに表示するテキスト
		WS_VISIBLE,				// ウィンドウ・スタイル
		CW_USEDEFAULT,			// ウィンドウの表示位置 (水平)
		CW_USEDEFAULT,			//                      (垂直)
		CW_USEDEFAULT,			// ウィンドウの大きさ   (幅)
		CW_USEDEFAULT,			//                      (高さ)
		NULL,					// 親ウィンドウのハンドル
		NULL,					// ウィンドウ・クラスのメニューを使用
		hInstance,				// アプリケーション・インスタンスのハンドル
		NULL					// ウィンドウ作成データのアドレス
	);
	if (!ghWnd)
		return FALSE;			// ウィンドウの作成に失敗

	ShowWindow(ghWnd, nCmdShow);	// ウィンドウ表示状態の設定
	UpdateWindow(ghWnd);			// クライアント領域の更新

	return TRUE;
}

/* =====================================================================
	ウィンドウ・プロシージャ
===================================================================== */
void set_colbutton(void)
{
	if (put_colbutton_flag==TRUE)
		BitBlt(GetDC(NULL), colbutton_x, colbutton_y, 14,14, colbuttonDC, 0,0, SRCCOPY);
}

void make_command_bar(HWND hWnd)
{
	static int b_pen[2]={BM_PEN_SEL,BM_PEN_USEL};
	static int b_era[2]={BM_ERASE_SEL,BM_ERASE_USEL};
	
	int		i,j;
	BYTE	*p=colbuttonBuf;
	for (j=0;j<14;j++)
		for (i=0;i<16;i++) {
			*p++=init_color[pcolor][0];
			*p++=init_color[pcolor][1];
			*p++=init_color[pcolor][2];
		}
	col_counter=5;

	switch (toolpic) {
		case TOOLPIC_PEN:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL, 0)); break;
		case TOOLPIC_ERASE:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+1, 0)); break;
		case TOOLPIC_SPUIT:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+2, 0)); break;
		case TOOLPIC_HAND:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+3, 0)); break;
		case TOOLPIC_PAINT:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+4, 0)); break;
		case TOOLPIC_FONT:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+5, 0)); break;
		case TOOLPIC_LINE:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+7, 0)); break;
		case TOOLPIC_RECT:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+8, 0)); break;
		case TOOLPIC_RECTFILL:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+9, 0)); break;
		case TOOLPIC_CIRCLE:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+10, 0)); break;
		case TOOLPIC_CIRCLEFILL:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+11, 0)); break;
		case TOOLPIC_COPY_RECT:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+12, 0)); break;
		case TOOLPIC_COPY_FREE:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+13, 0)); break;
		case TOOLPIC_COPY_MAGICWAND:
			SendMessage(ghWndCB, TB_CHANGEBITMAP, IDM_TOOLBUTTON, MAKELPARAM(BM_TOOL+14, 0)); break;
	}

	SendMessage(ghWndCB, TB_CHANGEBITMAP, 
		IDM_PENBUTTON, MAKELPARAM(b_pen[region_flag==PEN ? 0 : 1]+pstyle, 0));
//	SendMessage(ghWndCB, TB_CHANGEBITMAP, 
//		IDM_COLBUTTON, MAKELPARAM(BM_COLOR+pcolor, 0));
	SendMessage(ghWndCB, TB_CHANGEBITMAP, 
		IDM_PATBUTTON, MAKELPARAM(BM_PATTERN+(ppat<=7 ? pat_inv+ppat : 16), 0));
	SendMessage(ghWndCB, TB_CHANGEBITMAP, 
		IDM_ERABUTTON, MAKELPARAM(b_era[region_flag==ERASE ? 0 : 1]+perase, 0));
	SendMessage(ghWndCB, TB_CHANGEBITMAP, 
		IDM_MAGBUTTON, MAKELPARAM(BM_LOOPE+loope_num, 0));

	InvalidateRect(hWnd,NULL,FALSE);
	set_colbutton();
}
void menu_check(void) 
{
/*
	HMENU	hm;
	int		i;
	static UINT flag[]={MF_UNCHECKED,MF_CHECKED};
	static UINT lmenu[]={IDM_LOOPE_X1,IDM_LOOPE_X2,IDM_LOOPE_X3,IDM_LOOPE_X4,IDM_LOOPE_X8};
	static int	lmag[]={1,2,3,5,8};

	hm=CommandBar_GetMenu(ghWndCB, 0);
	
	for (i=0;i<5;i++) 
		CheckMenuItem(hm,lmenu[i],flag[lmag[i]==loope_mag ? 1 : 0]);
*/
}


void jzp_timer(HWND hWnd)
{
	if (active_flag==FALSE) return;
	if (force_24_flag==TRUE) return;
	tm++;
	if (region_flag==SELECT || region_flag==SELECT_END || region_flag==DRAG ||
		region_flag==SELECT_FREE || region_flag==SELECT_FREE_END)
		InvalidateRect(hWnd,NULL,FALSE);

	if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
		if (!pspc_flag) {
			if (GetKeyState(VK_LSHIFT) || GetKeyState(VK_RSHIFT)) {
				if (action_button_flag==FALSE) {
					action_button_flag=TRUE;
					InvalidateRect(hWnd,NULL,FALSE);
				}
			} else {
				if (action_button_flag==TRUE) {
					action_button_flag=FALSE;
					InvalidateRect(hWnd,NULL,FALSE);
				}
			}				
		}
		if (line_tm>0) {
			if (tm-line_tm>=1) {
				line_tm=-1;
				line_end(hWnd);
			}	
		}
	}
	if (erase_put_flag) {
			RECT rc;	 
			int	p=style[1][pstyle];
			rc.left=erase_x*loope_mag;
			rc.top=erase_y*loope_mag+cbh;	
			rc.right=(erase_x+p)*loope_mag;
			rc.bottom=(erase_y+p)*loope_mag+cbh;
			InvalidateRect(hWnd,&rc,FALSE);
	}

	if (col_counter>=0) {
		set_colbutton();
		col_counter--;
	}
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMessage,
						 WPARAM wParam, LPARAM lParam)
{
	int		x,y,sx,sy,ex,ey,i,j;
	switch (uMessage) {
		case WM_TIMER:		jzp_timer(hWnd);		break;

		case WM_CREATE:		OnCreate(hWnd, wParam, lParam);		break;
		case WM_DESTROY:	OnDestroy(hWnd, wParam, lParam);	break;
		case WM_PAINT:		OnPaint(hWnd, wParam, lParam);		break;

		case WM_COMMAND: 
			switch (wParam) {
			// ファイル・メニュー
				case IDM_CLEAR:		
					if (page_clear(hWnd)==TRUE) {
						overwrite_flag=FALSE;
						modified_flag=0;
						InvalidateRect(hWnd,NULL,FALSE);
					}
					break;
				case IDM_EXIT:		OnFileExit(hWnd);		break;

			// ヘルプ・メニュー
				case IDM_JZV_LINK:
					called_jzv_flag=TRUE;
					jzv_link(hWnd);
					overwrite_flag=TRUE;
					break;
				case IDM_UNDO:		do_undo(hWnd);			break;
				case IDM_LOAD:
					put_colbutton_flag=FALSE;
					load_bmp(hWnd);
					put_colbutton_flag=TRUE;
					col_counter=5;
					break;
				case IDM_SAVE:		
					put_colbutton_flag=FALSE;
					save_bmp(hWnd);			
					put_colbutton_flag=TRUE;
					col_counter=5;
					break; 
				case IDM_SAVE_CLIPBOARD:		
					sx=(region_sx<region_ex) ? region_sx : region_ex;
					sy=(region_sy<region_ey) ? region_sy : region_ey;
					ex=(region_sx>region_ex) ? region_sx : region_ex;
					ey=(region_sy>region_ey) ? region_sy : region_ey;
					put_colbutton_flag=FALSE;
					save_clipboard(hWnd,sx,sy,ex,ey);			
					put_colbutton_flag=TRUE;
					col_counter=5;
					break; 
				case IDM_OVERWRITE:	force_save();	break; 

				case IDM_ABOUT:		
					OnHelp(hWnd, wParam, lParam);		
					col_counter=5;
					break;

				case IDM_RECT_COPY:	
					if (rect_flag==TRUE) {
						sx=(region_sx<region_ex) ? region_sx : region_ex;
						sy=(region_sy<region_ey) ? region_sy : region_ey;
						ex=(region_sx>region_ex) ? region_sx : region_ex;
						ey=(region_sy>region_ey) ? region_sy : region_ey;
						for (y=0;y<winHeight;y++)
							for (x=0;x<winWidth;x++)
								set_pixel(freeareaBuf,x,y,0);
						for (y=sy;y<=ey;y++)
							for (x=sx;x<=ex;x++)
								set_pixel(freeareaBuf,x,y,1);
						for (y=1;y<winHeight-2;y++)
							for (x=1;x<winWidth-2;x++) {
								if (get_pixel(freeareaBuf,x,y)==1) {
									if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
									if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
									if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
									if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
									set_pixel(freeareaBuf,x,y,3);
								}
							}
						rect_flag=FALSE;
						clipboard_save_flag=TRUE;
					} else {
						free_paste_start();
						clipboard_save_flag=FALSE;
					}
					select_x=select_dx=0;
					select_y=select_dy=0;
					cut_flag=FALSE;
					rotate_flag=0;
					region_flag=DRAG;  
					clipboard_flag=TRUE;
					InvalidateRect(hWnd,NULL,FALSE);
					break;	
				case IDM_RECT_CUT:
					if (rect_flag==TRUE) {
						sx=(region_sx<region_ex) ? region_sx : region_ex;
						sy=(region_sy<region_ey) ? region_sy : region_ey;
						ex=(region_sx>region_ex) ? region_sx : region_ex;
						ey=(region_sy>region_ey) ? region_sy : region_ey;
						for (y=0;y<winHeight;y++) 
							for (x=0;x<winWidth;x++)
								set_pixel(freeareaBuf,x,y,0);
						for (y=sy;y<=ey;y++)
							for (x=sx;x<=ex;x++)
								set_pixel(freeareaBuf,x,y,1);
						for (y=1;y<winHeight-2;y++)
							for (x=1;x<winWidth-2;x++) {
								if (get_pixel(freeareaBuf,x,y)==1) {
									if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
									if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
									if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
									if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
									set_pixel(freeareaBuf,x,y,3);
								}
							}
						rect_flag=FALSE;
						clipboard_save_flag=TRUE;
					} else {
						free_paste_start();
						clipboard_save_flag=FALSE;
					}
					select_x=select_dx=0;
					select_y=select_dy=0;
					cut_flag=TRUE;
					undo_store(hWnd);
					cut_region(hWnd);
					modified_flag=1;
					rotate_flag=0;
					region_flag=DRAG;
					clipboard_flag=TRUE;
					InvalidateRect(hWnd,NULL,FALSE);
					break;	
				case IDM_PASTE:		
					select_dx=0;
					select_dy=0;
					region_flag=DRAG;
					toolpic=rect_flag ? TOOLPIC_COPY_RECT : TOOLPIC_COPY_FREE;
					make_command_bar(hWnd);
					break;	
				case IDM_DO_PASTE:		
					paste(hWnd);
					modified_flag=1;
					select_dx=0;
					select_dy=0;
					break;	
				case IDM_PASTE_CANSEL:	paste_cansel(hWnd);	break;	

				case IDM_SELECT_REGION:	
					region_flag=SELECT_START;
					start_select_rect_flag=AREA_RECT;
					toolpic=TOOLPIC_COPY_RECT;
					make_command_bar(hWnd);		
					InvalidateRect(hWnd,NULL,FALSE);
					break;	
				case IDM_SELECT_FREE:
					region_flag=SELECT_FREE_START;
					start_select_rect_flag=AREA_FREE;
					toolpic=TOOLPIC_COPY_FREE;
					make_command_bar(hWnd);		
					InvalidateRect(hWnd,NULL,FALSE);
					break;	
				case IDM_SELECT_ALL:
					region_flag=SELECT_START;
					start_select_rect_flag=AREA_ALL;
					toolpic=TOOLPIC_COPY_RECT;
					select_all(hWnd);
					make_command_bar(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;	
				case IDM_MAGICWAND:
					region_flag=SELECT;
					start_select_rect_flag=AREA_MAGICWAND;
					toolpic=TOOLPIC_COPY_MAGICWAND;
					select_magicwand(hWnd);
					make_command_bar(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;	

				case IDM_SPUIT:
					region_flag=SPUIT;
					toolpic=TOOLPIC_SPUIT;
					make_command_bar(hWnd);
					break;
				case IDM_HAND:
					hand_x=hand_dx=0;
					hand_y=hand_dy=0;
					hand_lx=loope_x;
					hand_ly=loope_y;
					hand_x=-1;
					region_flag=HAND;
					toolpic=TOOLPIC_HAND;
					make_command_bar(hWnd);
					break;
				case IDM_PAINT:		
					region_flag=PAINT;
					toolpic=TOOLPIC_PAINT;
					make_command_bar(hWnd);
					undo_store(hWnd);
					modified_flag=1;
					break;	

				case IDM_FLIP_VERTICAL:
					rotate_flag=rotate_table[rotate_flag][3];
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_FLIP_HORIZONTAL:
					rotate_flag=rotate_table[rotate_flag][2];
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_ROTATE_LEFT:
					rotate_flag=rotate_table[rotate_flag][1];
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_ROTATE_RIGHT:
					rotate_flag=rotate_table[rotate_flag][0];
					InvalidateRect(hWnd,NULL,FALSE);
					break;
 
				case IDM_CL_INVERSE:
					clipboard_inverse(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_CL_TRACEEDGE: 
					clipboard_lasso(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_CL_OMITWHITE:
					clipboard_magicwand(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case IDM_CL_EXPAND:
					clipboard_bold(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;

				case IDM_PALETTE_LOAD:
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							tmp_color[j][i]=init_color[j][i];
					tmp_col_sel=pcolor;
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode_tmp[j][i]=conv_mode[j][i];
					for (j=0;j<16;j++)
						cconv_tmp[j]=cconv[j];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode_tmp[j][i]=color_mode[j][i];
					for (j=0;j<16;j++)
						tmp_area_color[j]=area_color[j];
					color_load_mode=COLOR_LOAD_START;
					color_bg_mode=COLOR_BG_NONE;
					color_set_mode=COLOR_SET_NONE;
					color_conv_mode=COLOR_CONV_NONE;
					color_mask_mode=COLOR_MODE_NONE;
					color_area_mode=COLOR_AREA_NONE;
					color_load_open_flag=FALSE;
//					load_palette(hWnd);
					init_color_load_dialog();
					InvalidateRect(hWnd,NULL,FALSE);
					break; 

				case IDM_BGCOLOR:
					color_load_mode=COLOR_LOAD_NONE;
					color_bg_mode=COLOR_BG_START;
					color_set_mode=COLOR_SET_NONE;
					color_conv_mode=COLOR_CONV_NONE;
					color_mask_mode=COLOR_MODE_NONE;
					color_area_mode=COLOR_AREA_NONE;
					tmp_bgc_color=bgc;
					init_color_bg_dialog(tmp_bgc_color);
					InvalidateRect(hWnd,NULL,FALSE);
					break; 

				case IDM_COLSET: 
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							tmp_color[j][i]=init_color[j][i];
					tmp_col_sel=pcolor;
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode_tmp[j][i]=conv_mode[j][i];
					for (j=0;j<16;j++)
						cconv_tmp[j]=cconv[j];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode_tmp[j][i]=color_mode[j][i];
					for (j=0;j<16;j++)
						tmp_area_color[j]=area_color[j];
					color_load_mode=COLOR_LOAD_NONE;
					color_bg_mode=COLOR_BG_NONE;
					color_set_mode=COLOR_SET_START;
					color_conv_mode=COLOR_CONV_NONE;
					color_mask_mode=COLOR_MODE_NONE;
					color_area_mode=COLOR_AREA_NONE;
					init_color_set_dialog();
					InvalidateRect(hWnd,NULL,FALSE);
					break;

				case IDM_COLOR_CONV:
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode_tmp[j][i]=conv_mode[j][i];
					for (j=0;j<16;j++)
						cconv_tmp[j]=cconv[j];
					color_load_mode=COLOR_LOAD_NONE;
					color_bg_mode=COLOR_BG_NONE;
					color_set_mode=COLOR_SET_NONE;
					color_conv_mode=COLOR_CONV_START;
					color_mask_mode=COLOR_MODE_NONE;
					color_area_mode=COLOR_AREA_NONE;
					color_row=pcolor;
					init_color_conv_dialog(color_row,color_column);
					InvalidateRect(hWnd,NULL,FALSE);
					break;

				case IDM_COLOR_OPTION:
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode_tmp[j][i]=color_mode[j][i];
					color_load_mode=COLOR_LOAD_NONE;
					color_bg_mode=COLOR_BG_NONE;
					color_conv_mode=COLOR_CONV_NONE;
					color_set_mode=COLOR_SET_NONE;
					color_mask_mode=COLOR_MODE_START;
					color_area_mode=COLOR_AREA_NONE;
					color_row=pcolor;
					init_color_mode_dialog();
					InvalidateRect(hWnd,NULL,FALSE);
					break;
					
				case IDM_SELAREACOLOR:
					for (j=0;j<16;j++)
						tmp_area_color[j]=area_color[j];
					color_load_mode=COLOR_LOAD_NONE;
					color_bg_mode=COLOR_BG_NONE;
					color_set_mode=COLOR_SET_NONE;
					color_conv_mode=COLOR_CONV_NONE;
					color_mask_mode=COLOR_MODE_NONE;
					color_area_mode=COLOR_AREA_START;
					init_color_area_dialog();
					InvalidateRect(hWnd,NULL,FALSE);
					break;

				case IDM_FONT:	
					rotate_flag=0;
					region_flag=FONT;
					toolpic=TOOLPIC_FONT;
					make_command_bar(hWnd);
//					input_text(hWnd);	
					break;	
				
				case IDM_PENBUTTON:	pen_button(hWnd);	break;	
//				case IDM_COLBUTTON:	col_button(hWnd);	break;	
//				case IDM_PATBUTTON:	pat_button(hWnd);	break;	
				case IDM_ERABUTTON:	erase_button(hWnd);	break;	
				case IDM_LOOPE_X1:	mag_button(hWnd,0);	break;	
				case IDM_LOOPE_X2:	mag_button(hWnd,1);	break;	
				case IDM_LOOPE_X3:	mag_button(hWnd,2);	break;	
				case IDM_LOOPE_X4:	mag_button(hWnd,3);	break;	
				case IDM_LOOPE_X8:	mag_button(hWnd,4);	break;	
				case IDM_B_PAT0:	pat_button(hWnd,0);	break;	
				case IDM_B_PAT1:	pat_button(hWnd,1);	break;	
				case IDM_B_PAT2:	pat_button(hWnd,2);	break;	
				case IDM_B_PAT3:	pat_button(hWnd,3);	break;	
				case IDM_B_PAT4:	pat_button(hWnd,4);	break;	
				case IDM_B_PAT5:	pat_button(hWnd,5);	break;	
				case IDM_B_PAT6:	pat_button(hWnd,6);	break;	
				case IDM_B_PAT7:	pat_button(hWnd,7);	break;	
				case IDM_B_PATC:	pat_button(hWnd,8);	break;	
				case IDM_B_COL_0:	col_button(hWnd,0);	break;	
				case IDM_B_COL_1:	col_button(hWnd,1);	break;	
				case IDM_B_COL_2:	col_button(hWnd,2);	break;	
				case IDM_B_COL_3:	col_button(hWnd,3);	break;	
				case IDM_B_COL_4:	col_button(hWnd,4);	break;	
				case IDM_B_COL_5:	col_button(hWnd,5);	break;	
				case IDM_B_COL_6:	col_button(hWnd,6);	break;	
				case IDM_B_COL_7:	col_button(hWnd,7);	break;	
				case IDM_B_COL_8:	col_button(hWnd,8);	break;	
				case IDM_B_COL_9:	col_button(hWnd,9);	break;	
				case IDM_B_COL_10:	col_button(hWnd,10);	break;	
				case IDM_B_COL_11:	col_button(hWnd,11);	break;	
				case IDM_B_COL_12:	col_button(hWnd,12);	break;	
				case IDM_B_COL_13:	col_button(hWnd,13);	break;	
				case IDM_B_COL_14:	col_button(hWnd,14);	break;	
				case IDM_B_COL_15:	col_button(hWnd,15);	break;	

				case IDM_PATINV:
					if (pat_inv==0)	pat_inv=8;
					else			pat_inv=0;
					pat_button(hWnd,ppat);
					InvalidateRect(hWnd,NULL,FALSE);
					break;	

				case IDM_LOAD_CUSTUM_TONE:
					load_custum_tone(hWnd);
					pat_button(hWnd,ppat);
					InvalidateRect(hWnd,NULL,FALSE);
					break;	


				case IDM_LOOPE_ORG:
					if (loope_orginal_size_view_flag[loope_num]==TRUE)
						loope_orginal_size_view_flag[loope_num]=FALSE;
					else
						loope_orginal_size_view_flag[loope_num]=TRUE;
					InvalidateRect(hWnd,NULL,FALSE);
					break;	

				case IDM_COM_LINE:
					region_flag=LINE;
					toolpic=TOOLPIC_LINE;
					make_command_bar(hWnd);
					line_start(hWnd);
					break;
				case IDM_COM_RECT:
					region_flag=RECTANGLE;
					fill_flag=FALSE;
					toolpic=TOOLPIC_RECT;
					make_command_bar(hWnd);
					line_start(hWnd);
					break;
				case IDM_COM_RECTFILL:
					region_flag=RECTANGLE;
					fill_flag=TRUE;
					toolpic=TOOLPIC_RECTFILL;
					make_command_bar(hWnd);
					line_start(hWnd);
					break;
				case IDM_COM_CIRCLE:
					region_flag=CIRCLE;
					fill_flag=FALSE;
					toolpic=TOOLPIC_CIRCLE;
					make_command_bar(hWnd);
					line_start(hWnd);
					break;
				case IDM_COM_CIRCLEFILL:
					region_flag=CIRCLE;
					fill_flag=TRUE;
					toolpic=TOOLPIC_CIRCLEFILL;
					make_command_bar(hWnd); 
					line_start(hWnd);
					break;
				case IDM_RETURN:
					if (action_button_flag==FALSE) {
						action_button_flag=TRUE;
						InvalidateRect(hWnd,NULL,FALSE);
					}
						pstyle=2;
					break;
				case IDM_LINE_ADJUST_0:
					line_adjust_max=0;
					break;
				case IDM_LINE_ADJUST_1:
					line_adjust_max=1;
					break;
				case IDM_LINE_ADJUST_2:
					line_adjust_max=2;
					break;
			}
			break;
		case WM_ACTIVATE: 
			if (LOWORD(wParam)==WA_ACTIVE) {
				active_flag=TRUE;
			} else {
				active_flag=FALSE;
			}
			break;
		case WM_KEYDOWN:
			switch (LOWORD(wParam)) {
				case VK_UP:
					loope_y-=winHeight/loope_mag/loope_move;
					if (loope_y<-winHeight/loope_mag/2-1)
						loope_y=-winHeight/loope_mag/2-1;
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_DOWN:
					loope_y+=winHeight/loope_mag/loope_move;
					if (loope_y>winHeight-winHeight/loope_mag/2)
						loope_y=winHeight-winHeight/loope_mag/2;
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_LEFT:
					loope_x-=winWidth/loope_mag/loope_move;
					if (loope_x<-winWidth/loope_mag/2)
						loope_x=-winWidth/loope_mag/2;
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_RIGHT:
					loope_x+=winWidth/loope_mag/loope_move;
					if (loope_x>winWidth-winWidth/loope_mag/2)
						loope_x=winWidth-winWidth/loope_mag/2;
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_F23:
					if (action_button_flag==FALSE) {
						action_button_flag=TRUE;
						InvalidateRect(hWnd,NULL,FALSE);
					}
					break;
			}
			break;
		case WM_KEYUP:
			switch (LOWORD(wParam))	{
				case VK_RETURN:
					action_button_flag=FALSE;
					InvalidateRect(hWnd,NULL,FALSE);
					break;
			}
			break;
		case WM_LBUTTONDOWN:	mouse_down(hWnd,wParam, lParam);	break;
		case WM_MOUSEMOVE:		mouse_move(hWnd,wParam, lParam);	break;
		case WM_LBUTTONUP:		mouse_up(hWnd,wParam, lParam);		break;
		case WM_INITMENUPOPUP:
			{
				BOOL mode;
				HMENU hMenu = CommandBar_GetMenu( GetDlgItem(hWnd,1), 0 );
//				EnableMenuItem(hMenu,IDM_RECT_COPY,MF_BYCOMMAND | MF_GRAYED);

				mode= region_flag==DRAG ? MF_ENABLED : MF_GRAYED;
				EnableMenuItem(hMenu,IDM_FLIP_VERTICAL,		MF_BYCOMMAND | mode);
				EnableMenuItem(hMenu,IDM_FLIP_HORIZONTAL,	MF_BYCOMMAND | mode);
				EnableMenuItem(hMenu,IDM_ROTATE_LEFT,		MF_BYCOMMAND | mode);
				EnableMenuItem(hMenu,IDM_ROTATE_RIGHT,		MF_BYCOMMAND | mode);

				if (region_flag==SELECT || 
//					region_flag==SELECT_START || 
					region_flag==SELECT_END || 
//					region_flag==SELECT_FREE_START || 
					region_flag==SELECT_FREE
					|| region_flag==SELECT_FREE_END
				)
					mode=MF_ENABLED ; 
				else mode=MF_GRAYED;
				EnableMenuItem(hMenu,IDM_RECT_CUT,		MF_BYCOMMAND | mode);
				EnableMenuItem(hMenu,IDM_RECT_COPY,		MF_BYCOMMAND | mode);

				mode= (clipboard_flag==TRUE) ? MF_ENABLED : MF_GRAYED;
				EnableMenuItem(hMenu,IDM_PASTE,		MF_BYCOMMAND | mode);

				mode= (clipboard_save_flag==TRUE) ? MF_ENABLED : MF_GRAYED;
				EnableMenuItem(hMenu,IDM_SAVE_CLIPBOARD,	MF_BYCOMMAND | mode);

				mode= (clipboard_flag==TRUE && region_flag==DRAG) ? MF_ENABLED : MF_GRAYED;
				EnableMenuItem(hMenu,IDM_DO_PASTE,	MF_BYCOMMAND | mode);

				mode= (overwrite_flag) ? MF_ENABLED : MF_GRAYED;
				EnableMenuItem(hMenu,IDM_OVERWRITE,	MF_BYCOMMAND | mode);

				CheckMenuItem(CommandBar_GetMenu(ghWndCB, 0),IDM_LINE_ADJUST_0,	MF_BYCOMMAND | (line_adjust_max==0 ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(CommandBar_GetMenu(ghWndCB, 0),IDM_LINE_ADJUST_1,	MF_BYCOMMAND | (line_adjust_max==1 ? MF_CHECKED : MF_UNCHECKED));
				CheckMenuItem(CommandBar_GetMenu(ghWndCB, 0),IDM_LINE_ADJUST_2,	MF_BYCOMMAND | (line_adjust_max==2 ? MF_CHECKED : MF_UNCHECKED));
/*
				hMenu = CommandBar_GetMenu( GetDlgItem(hWnd,1), 0 );
				mode= (loope_orginal_size_view_flag) ? MF_CHECKED : MF_UNCHECKED;
				mode=MF_CHECKED;
				CheckMenuItem(hMenu,IDM_LOOPE_ORG,	MF_BYCOMMAND | mode);
*/
			}
			break;
		case WM_NOTIFY: {
			LPNMHDR	lpnm=(LPNMHDR)lParam;
			LPNMTOOLBAR	lpnmtb=(LPNMTOOLBAR)lParam;
			RECT	rc;
			TPMPARAMS	tpm;
			HMENU	hMenu,hPopupMenu=NULL;
			BOOL	bRet=FALSE;
/*
				if (col_menu_flag==TRUE) {
					TCHAR	Message[200];
					rc.top=test_n*20+30;rc.bottom=rc.top+20;rc.left=0;rc.right=240;
					test_n++;
					wsprintf(Message,TEXT("[%d %d %d]"),lpnm->code,lpnm->idFrom,lpnmtb->iItem);
					DrawText(baseDC, Message, -1,&rc, DT_LEFT | DT_VCENTER);
					InvalidateRect(hWnd,NULL,FALSE);
				}
*/
			switch (lpnm->code) {
				case TBN_BEGINDRAG:
					switch((WPARAM)lpnmtb->iItem) {
					case IDM_COLBUTTON:
						if (col_menu_flag==FALSE) {
							SendMessage(lpnmtb->hdr.hwndFrom,TB_GETRECT,(WPARAM)lpnmtb->iItem,(LPARAM)&rc);
							MapWindowPoints(lpnmtb->hdr.hwndFrom,HWND_DESKTOP,(LPPOINT)&rc,2);
							tpm.cbSize=sizeof(TPMPARAMS);	
							tpm.rcExclude.top=rc.top;	
							tpm.rcExclude.left=rc.left;
							tpm.rcExclude.bottom=rc.bottom;	
							tpm.rcExclude.right=rc.right;	
							col_pulldown_x_pos=rc.left+30;
							TrackPopupMenuEx(col_menu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
							col_menu_flag=TRUE;
							return(FALSE);
						}
						else {
							SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_COLBUTTON,MAKELONG(FALSE, 0));
							col_menu_flag=FALSE;
							return(FALSE);
						}
					case IDM_PATBUTTON:
						if (pat_menu_flag==FALSE) {
							CheckMenuItem(pat_menu,IDM_PATINV,	MF_BYCOMMAND | ((pat_inv!=0) ? MF_CHECKED : MF_UNCHECKED));
							SendMessage(lpnmtb->hdr.hwndFrom,TB_GETRECT,(WPARAM)lpnmtb->iItem,(LPARAM)&rc);
							MapWindowPoints(lpnmtb->hdr.hwndFrom,HWND_DESKTOP,(LPPOINT)&rc,2);
							tpm.cbSize=sizeof(TPMPARAMS);	
							tpm.rcExclude.top=rc.top;	
							tpm.rcExclude.left=rc.left;
							tpm.rcExclude.bottom=rc.bottom;	
							tpm.rcExclude.right=rc.right;	
							TrackPopupMenuEx(pat_menu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
							pat_menu_flag=TRUE;
							return(FALSE);
						}
						else {
							SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_PATBUTTON,MAKELONG(FALSE, 0));
							pat_menu_flag=FALSE;
							return(FALSE);
						}
					case IDM_MAGBUTTON:
						if (loope_menu_flag==FALSE) {
							SendMessage(lpnmtb->hdr.hwndFrom,TB_GETRECT,(WPARAM)lpnmtb->iItem,(LPARAM)&rc);
							MapWindowPoints(lpnmtb->hdr.hwndFrom,HWND_DESKTOP,(LPPOINT)&rc,2);
							tpm.cbSize=sizeof(TPMPARAMS);	
							tpm.rcExclude.top=rc.top;	
							tpm.rcExclude.left=rc.left;
							tpm.rcExclude.bottom=rc.bottom;	
							tpm.rcExclude.right=rc.right;	
							hMenu=LoadMenu(ghInst,MAKEINTRESOURCE(IDR_LOOPE));
							CheckMenuItem(hMenu,IDM_LOOPE_ORG,	MF_BYCOMMAND | ((loope_orginal_size_view_flag[loope_num]) ? MF_CHECKED : MF_UNCHECKED));
							hPopupMenu=GetSubMenu(hMenu,0);
							TrackPopupMenuEx(hPopupMenu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
							DestroyMenu(hMenu);
							loope_menu_flag=TRUE;
							return(FALSE);
						}
						else {
							SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_MAGBUTTON,MAKELONG(FALSE, 0));
							loope_menu_flag=FALSE;
							return(FALSE);
						}
					case IDM_TOOLBUTTON:
						if (tool_menu_flag==FALSE) {
							SendMessage(lpnmtb->hdr.hwndFrom,TB_GETRECT,(WPARAM)lpnmtb->iItem,(LPARAM)&rc);
							MapWindowPoints(lpnmtb->hdr.hwndFrom,HWND_DESKTOP,(LPPOINT)&rc,2);
							tpm.cbSize=sizeof(TPMPARAMS);	
							tpm.rcExclude.top=rc.top;	
							tpm.rcExclude.left=rc.left;
							tpm.rcExclude.bottom=rc.bottom;	
							tpm.rcExclude.right=rc.right;	
//							hMenu=LoadMenu(ghInst,MAKEINTRESOURCE(IDR_TOOL));
//							hPopupMenu=GetSubMenu(hMenu,0);
//							TrackPopupMenuEx(hPopupMenu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
//							DestroyMenu(hMenu);
							TrackPopupMenuEx(tool_menu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
							tool_menu_flag=TRUE;
							return(FALSE);
						}
						else {
							SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_TOOLBUTTON,MAKELONG(FALSE, 0));
							tool_menu_flag=FALSE;
							return(FALSE);
						}

					}
			}
			break;
		}
		case WM_MEASUREITEM:
			{
				LPMEASUREITEMSTRUCT	lpMI=(LPMEASUREITEMSTRUCT)lParam;
				LPDRAWITEMSTRUCT lpDI=(LPDRAWITEMSTRUCT)lParam;
				if (IDM_B_COL_0<=(lpDI->itemID) &&  (lpDI->itemID)<=IDM_B_COL_15) {
					lpMI->itemWidth=10+22-7+12;
					lpMI->itemHeight=13;
					return TRUE;
				}
				if (IDM_B_COL_P_0<=(lpDI->itemID) &&  (lpDI->itemID)<=IDM_B_COL_P_15) {
					lpMI->itemWidth=13;
					lpMI->itemHeight=13;
					return TRUE;
				}
				lpMI->itemWidth=10+22+40-7;
				lpMI->itemHeight=22;
				return TRUE;
			}
		case WM_DRAWITEM:
			{
				RECT	rc;
				HDC		hdc;
				int		i,j,n,button,s;
				LPDRAWITEMSTRUCT lpDI=(LPDRAWITEMSTRUCT)lParam;
				rc=lpDI->rcItem;
				hdc=lpDI->hDC;
				button=4;
				n=6;
				switch(lpDI->itemID) {
					case IDM_B_PEN0:	button=0;	n=0;	break;
					case IDM_B_PEN1:	button=0;	n=1;	break;
					case IDM_B_PEN2:	button=0;	n=2;	break;
					case IDM_B_COL_0:	button=1;	n=0;	break;
					case IDM_B_COL_1:	button=1;	n=1;	break;
					case IDM_B_COL_2:	button=1;	n=2;	break;
					case IDM_B_COL_3:	button=1;	n=3;	break;
					case IDM_B_COL_4:	button=1;	n=4;	break;
					case IDM_B_COL_5:	button=1;	n=5;	break;
					case IDM_B_COL_6:	button=1;	n=6;	break;
					case IDM_B_COL_7:	button=1;	n=7;	break;
					case IDM_B_COL_8:	button=1;	n=8;	break;
					case IDM_B_COL_9:	button=1;	n=9;	break;
					case IDM_B_COL_10:	button=1;	n=10;	break;
					case IDM_B_COL_11:	button=1;	n=11;	break;
					case IDM_B_COL_12:	button=1;	n=12;	break;
					case IDM_B_COL_13:	button=1;	n=13;	break;
					case IDM_B_COL_14:	button=1;	n=14;	break;
					case IDM_B_COL_15:	button=1;	n=15;	break;
					case IDM_B_PAT0:	button=2;	n=0;	break;
					case IDM_B_PAT1:	button=2;	n=1;	break;
					case IDM_B_PAT2:	button=2;	n=2;	break;
					case IDM_B_PAT3:	button=2;	n=3;	break;
					case IDM_B_PAT4:	button=2;	n=4;	break;
					case IDM_B_PAT5:	button=2;	n=5;	break;
					case IDM_B_PAT6:	button=2;	n=6;	break;
					case IDM_B_PAT7:	button=2;	n=7;	break;
					case IDM_B_PATC:	button=2;	n=8;	break;
					case IDM_B_ERA0:	button=3;	n=0;	break;
					case IDM_B_ERA1:	button=3;	n=1;	break;
					case IDM_B_ERA2:	button=3;	n=2;	break;
					case IDM_COM_LINE:		button=4;	n=7;	break;
					case IDM_COM_RECT:		button=4;	n=8;	break;
					case IDM_COM_RECTFILL:	button=4;	n=9;	break;
					case IDM_COM_CIRCLE:	button=4;	n=10;	break;
					case IDM_COM_CIRCLEFILL:	button=4;	n=11;	break;
					case IDM_PENBUTTON:	button=4;	n=0;	break;
					case IDM_ERABUTTON:	button=4;	n=1;	break;
					case IDM_SPUIT:		button=4;	n=2;	break;
					case IDM_HAND:		button=4;	n=3;	break;
					case IDM_PAINT:		button=4;	n=4;	break;
					case IDM_FONT:		button=4;	n=5;	break;
					case IDM_LINE:		button=4;	n=6;	break;
				}
				if (lpDI->itemState==ODS_SELECTED && button==1) {
					colsel_pflag=FALSE;
					if ((int)(GetMessagePos()&0xffff)>col_pulldown_x_pos)	colsel_pflag=TRUE;
					if (colsel_pflag==TRUE)
						color_mode[pcolor][n]=(color_mode[pcolor][n]==TRUE) ? FALSE : TRUE;
				}
				for (j=0;j<22;j++) 
					for (i=0;i<10+22+50;i++) {
						set_pixel(tmpBuf,i,j,lpDI->itemState==ODS_SELECTED ? 0 : 2);
						set_pixel_24(tmp24Buf,i,j,lpDI->itemState==ODS_SELECTED ? 16+0 : 16+2);
					}
				for (j=0;j<22;j++) 
					for (i=0;i<10+22+50;i++) {
						set_pixel(tmpBuf,i,j+22,2);
						set_pixel_24(tmp24Buf,i,j+22,16+2);
					}
				if (button==4) {
					static TCHAR *line_str[]={TEXT("Pen"),TEXT("Erase"),TEXT("Spuit"),TEXT("Hand"),TEXT("Paint"),TEXT("Font"),TEXT("Line"),
											TEXT("Line"),TEXT("Rect"),TEXT("F Rect"),TEXT("Circle"),TEXT("F Circle")};
					HBITMAP	hb;
					HDC		hmdc;
					RECT	frc;
					hb=LoadBitmap(ghInst,MAKEINTRESOURCE(IDR_BM_TOOL));
				    hmdc = CreateCompatibleDC(tmpDC);
					SelectObject(hmdc, hb);
				    BitBlt(tmpDC, 2, 5+22, 2+16-1, 5+16-1+22, hmdc, n*16, 0, SRCCOPY);
				    DeleteDC(hmdc);
					for (j=0;j<22;j++) 
						for (i=0;i<10+22+50;i++) {
							BYTE	gp=get_pixel(tmpBuf,i,j+22);
							if (lpDI->itemState==ODS_SELECTED) gp=3-gp;
							set_pixel(tmpBuf,i,j,gp);
						}
					frc.left=22;
					frc.right=22+100;
					frc.top=22+4;
					frc.bottom=22+22;
					DrawText(tmpDC, line_str[n], -1,&frc, DT_LEFT | DT_VCENTER);
					for (j=0;j<22;j++) 
						for (i=0;i<10+22+50;i++) {
							BYTE	gp=get_pixel(tmpBuf,i,j+22);
							if (gp==3) gp=2;
							if (lpDI->itemState==ODS_SELECTED) gp=2-gp;
							set_pixel(tmpBuf,i,j,gp);
						}
				}
				if (button==1) {
					for (j=0;j<10;j++) {
						for (i=0;i<18;i++) {
							if (i==0 || i==17 || j==0 || j==9)	{
								set_pixel_24(tmp24Buf,10+i+2,j+2,16+0);
							} else {
								set_pixel_24(tmp24Buf,10+i+2,j+2,n);
							}
						}
					}
					{
						int p=pcolor;
						BYTE c=(lpDI->itemState==ODS_SELECTED) ? 3 : 0;
						static TCHAR col_str[16];
						if (p==n) {
							set_pixel_24(tmp24Buf,5,6,16+c);
							set_pixel_24(tmp24Buf,5,7,16+c);
							set_pixel_24(tmp24Buf,5,8,16+c);
							set_pixel_24(tmp24Buf,6,6,16+c);
							set_pixel_24(tmp24Buf,6,7,16+c);
							set_pixel_24(tmp24Buf,6,8,16+c);
							set_pixel_24(tmp24Buf,7,6,16+c);
							set_pixel_24(tmp24Buf,7,7,16+c);
							set_pixel_24(tmp24Buf,7,8,16+c);
						}
						if (color_mode[p][n]==TRUE) {
							set_pixel_24(tmp24Buf,35,6,16+c);
							set_pixel_24(tmp24Buf,35,7,16+c);
							set_pixel_24(tmp24Buf,35,8,16+c);
							set_pixel_24(tmp24Buf,36,6,16+c);
							set_pixel_24(tmp24Buf,36,7,16+c);
							set_pixel_24(tmp24Buf,36,8,16+c);
							set_pixel_24(tmp24Buf,37,6,16+c);
							set_pixel_24(tmp24Buf,37,7,16+c);
							set_pixel_24(tmp24Buf,37,8,16+c);
						}
					}
				}
				if (button<4 && button!=1) {
					for (j=0;j<18;j++) {
						for (i=0;i<18;i++) {
							if (i==0 || i==17 || j==0 || j==17)	{
								set_pixel(tmpBuf,10+i+2,j+2,0);
							} else {
								if (button==0) 
									set_pixel(tmpBuf,10+i+2,j+2,2);
								if (button==1) {
									if (n<=3)	set_pixel(tmpBuf,10+i+2,j+2,n);
									else		set_pixel(tmpBuf,10+i+2,j+2,2);
								}
								if (button==2)
									if (n<=7)
										set_pixel(tmpBuf,10+i+2,j+2,brush_pat_4[pat_inv+n][(i-1)%4][(j-1)%4] ? 0 : 2);
									else
										set_pixel(tmpBuf,10+i+2,j+2,brush_pat_custum[i-1][j-1] ? 0 : 2);
								if (button==3) 
									set_pixel(tmpBuf,10+i+2,j+2,0);
							}
						}
					}
					if (button==2) {
						int p=ppat;
						BYTE c=(lpDI->itemState==ODS_SELECTED) ? 2 : 0;
						static TCHAR *pat_str[]={TEXT("100%"),TEXT(""),TEXT("75%"),TEXT(""),TEXT("50%"),TEXT(""),TEXT("25%"),TEXT(""),TEXT("CT")};
						static TCHAR *pat_str_inv[]={TEXT(""),TEXT("75%"),TEXT(""),TEXT("50%"),TEXT(""),TEXT("25%"),TEXT(""),TEXT("0%"),TEXT("CT")};
						RECT frc;
						if (p==n) {
							set_pixel(tmpBuf,5,10,c);
							set_pixel(tmpBuf,5,11,c);
							set_pixel(tmpBuf,5,12,c);
							set_pixel(tmpBuf,6,10,c);
							set_pixel(tmpBuf,6,11,c);
							set_pixel(tmpBuf,6,12,c);
							set_pixel(tmpBuf,7,10,c);
							set_pixel(tmpBuf,7,11,c);
							set_pixel(tmpBuf,7,12,c);
						}
						frc.left=10+22;
						frc.right=10+22+40-5;
						frc.top=22+4;
						frc.bottom=22+22;
						if (pat_inv==0)
							DrawText(tmpDC, pat_str[n], -1,&frc, DT_RIGHT | DT_CENTER);
						else
							DrawText(tmpDC, pat_str_inv[n], -1,&frc, DT_RIGHT | DT_CENTER);
						for (j=0;j<22;j++)
							for (i=frc.left;i<=frc.right;i++)
								if (get_pixel(tmpBuf,i,j+22)==0)
									set_pixel(tmpBuf,i,j,c);
					}
					if (button==0 || button==3) {
						s=style[button==0 ? 0:1][n];
						for (j=0;j<s;j++) 
							for (i=0;i<s;i++) {
								if (button==0) 
									set_pixel(tmpBuf,22+i+2+9-s/2,j+2+9-s/2,pen_pat[0][n] ? 0:2);
								else
									set_pixel(tmpBuf,22+i+2+9-s/2,j+2+9-s/2,pen_pat[1][n] ? 2:0);
							}
		
					}
				}
				rc=lpDI->rcItem;
				if (button==1)
					BitBlt(hdc,rc.left,rc.top,22+22+40+12,13,tmp24DC,0,0,SRCCOPY);
				else
					BitBlt(hdc,rc.left,rc.top,22+22+40,22,tmpDC,0,0,SRCCOPY);
			}
			return TRUE;
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	} 

	return 0;
}

/* =====================================================================
	ウィンドウ作成時の処理
===================================================================== */
void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC	hDC;
	RECT rc;
	TCHAR		Message[ 256 ] ;
	int	i,j; 

	// コマンドバーの生成
	ghWndCB = CommandBar_Create(ghInst, hWnd, 1);

	// アイコンの設定
	HICON hIcon = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_MAIN_ICON),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	SendMessage(hWnd, WM_SETICON, FALSE, (WPARAM)hIcon);

	// 画面のサイズを取得
	GetWindowRect(hWnd, &rc);

	cbh=CommandBar_Height(ghWndCB);
	winWidth  = rc.right - rc.left;
	winHeight = rc.bottom - rc.top - cbh;

	if (winWidth<=240) {
		pspc_flag=TRUE;
		CommandBar_InsertMenubar(ghWndCB, ghInst, IDR_MAIN_MENU, 0);
	} else {
		pspc_flag=FALSE;
		CommandBar_InsertMenubar(ghWndCB, ghInst, IDR_MAIN_H_MENU, 0);
//		CommandBar_AddAdornments(ghWndCB, CMDBAR_HELP, 0);
	}
	
//	CommandBar_AddAdornments(ghWndCB, CMDBAR_HELP, 0);
//	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_TOOLBAR1, 29, 16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_TOOL,			BM_NUM_TOOL,		16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_PEN_SEL,		BM_NUM_PEN_SEL,		16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_PEN_USEL,		BM_NUM_PEN_USEL,	16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_COLOR,		BM_NUM_COLOR,		16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_PATTERN,		BM_NUM_PATTERN,		16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_ERASE_SEL,	BM_NUM_ERASE_SEL,	16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_ERASE_USEL,	BM_NUM_ERASE_USEL,	16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_LOOPE,		BM_NUM_LOOPE,		16, 16 ) ;
	CommandBar_AddBitmap( ghWndCB, ghInst, IDR_BM_ETC,			BM_NUM_ETC,			16, 16 ) ;

//	LoadString(ghInst, IDS_STRING1, (LPTSTR)&Message, 8 - 1);
//	tb[1].iString=SendMessage(ghWndCB, TB_ADDSTRING, 0, (LPARAM)(LPTSTR)Message);

	CommandBar_AddButtons( ghWndCB, sizeof(tb) / sizeof(TBBUTTON), tb) ;
//	CommandBar_InsertButton(ghWndCB,1,tbSTDButton);
	menu_check();


	pat_menu=CreatePopupMenu();
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT0,(LPCTSTR)IDM_B_PAT0);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT1,(LPCTSTR)IDM_B_PAT1);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT2,(LPCTSTR)IDM_B_PAT2);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT3,(LPCTSTR)IDM_B_PAT3);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT4,(LPCTSTR)IDM_B_PAT4);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT5,(LPCTSTR)IDM_B_PAT5);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT6,(LPCTSTR)IDM_B_PAT6);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PAT7,(LPCTSTR)IDM_B_PAT7);
	AppendMenu(pat_menu,MF_OWNERDRAW,IDM_B_PATC,(LPCTSTR)IDM_B_PATC);
	AppendMenu(pat_menu,MF_SEPARATOR,0         ,(LPCTSTR)0);
	AppendMenu(pat_menu,MF_STRING   ,IDM_PATINV,(LPCTSTR)TEXT("Reverse"));
	col_menu=CreatePopupMenu();
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_0, (LPCTSTR)IDM_B_COL_0);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_1, (LPCTSTR)IDM_B_COL_1);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_2, (LPCTSTR)IDM_B_COL_2);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_3, (LPCTSTR)IDM_B_COL_3);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_4, (LPCTSTR)IDM_B_COL_4);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_5, (LPCTSTR)IDM_B_COL_5);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_6, (LPCTSTR)IDM_B_COL_6);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_7, (LPCTSTR)IDM_B_COL_7);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_8, (LPCTSTR)IDM_B_COL_8); 
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_9, (LPCTSTR)IDM_B_COL_9);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_10,(LPCTSTR)IDM_B_COL_10);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_11,(LPCTSTR)IDM_B_COL_11);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_12,(LPCTSTR)IDM_B_COL_12);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_13,(LPCTSTR)IDM_B_COL_13);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_14,(LPCTSTR)IDM_B_COL_14);
	AppendMenu(col_menu,MF_OWNERDRAW,IDM_B_COL_15,(LPCTSTR)IDM_B_COL_15);
	col_p_menu=CreatePopupMenu();
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_0, (LPCTSTR)IDM_B_COL_P_0);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_1, (LPCTSTR)IDM_B_COL_P_1);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_2, (LPCTSTR)IDM_B_COL_P_2);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_3, (LPCTSTR)IDM_B_COL_P_3);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_4, (LPCTSTR)IDM_B_COL_P_4);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_5, (LPCTSTR)IDM_B_COL_P_5);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_6, (LPCTSTR)IDM_B_COL_P_6);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_7, (LPCTSTR)IDM_B_COL_P_7);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_8, (LPCTSTR)IDM_B_COL_P_8); 
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_9, (LPCTSTR)IDM_B_COL_P_9);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_10,(LPCTSTR)IDM_B_COL_P_10);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_11,(LPCTSTR)IDM_B_COL_P_11);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_12,(LPCTSTR)IDM_B_COL_P_12);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_13,(LPCTSTR)IDM_B_COL_P_13);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_14,(LPCTSTR)IDM_B_COL_P_14);
	AppendMenu(col_p_menu,MF_OWNERDRAW,IDM_B_COL_P_15,(LPCTSTR)IDM_B_COL_P_15);
	line_menu=CreatePopupMenu();
	AppendMenu(line_menu,MF_OWNERDRAW,IDM_COM_LINE, (LPCTSTR)IDM_COM_LINE);
	AppendMenu(line_menu,MF_OWNERDRAW,IDM_COM_RECT, (LPCTSTR)IDM_COM_RECT);
	AppendMenu(line_menu,MF_OWNERDRAW,IDM_COM_RECTFILL, (LPCTSTR)IDM_COM_RECTFILL);
	AppendMenu(line_menu,MF_OWNERDRAW,IDM_COM_CIRCLE, (LPCTSTR)IDM_COM_CIRCLE);
	AppendMenu(line_menu,MF_OWNERDRAW,IDM_COM_CIRCLEFILL, (LPCTSTR)IDM_COM_CIRCLEFILL);
	tool_menu=CreatePopupMenu();
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_PENBUTTON, (LPCTSTR)IDM_PENBUTTON);
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_ERABUTTON, (LPCTSTR)IDM_ERABUTTON);
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_SPUIT, (LPCTSTR)IDM_SPUIT);
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_HAND, (LPCTSTR)IDM_HAND);
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_PAINT, (LPCTSTR)IDM_PAINT);
	AppendMenu(tool_menu,MF_OWNERDRAW,IDM_FONT, (LPCTSTR)IDM_FONT);
	AppendMenu(tool_menu,MF_POPUP|MF_OWNERDRAW,(UINT)line_menu, (LPCTSTR)IDM_LINE);
/*
	if (!(winWidth<=240)) {
		CommandBar_AddAdornments(ghWndCB, NULL, 0);
	}
*/
	// オフスクリーンの作成
	bmi.bmih.biSize = sizeof(bmi.bmih) ;
	bmi.bmih.biPlanes = 1 ;
	bmi.bmih.biCompression = BI_RGB ;
	bmi.bmih.biSizeImage = 0 ;
	bmi.bmih.biXPelsPerMeter = 0 ; 
	bmi.bmih.biYPelsPerMeter = 0 ;
	bmi.bmih.biClrUsed = 0 ;
	bmi.bmih.biClrImportant = 0 ;
	for (i=0;i<16;i++) {
		bmi.rgq[i].rgbBlue  = init_color[i][0];
		bmi.rgq[i].rgbGreen = init_color[i][1];
		bmi.rgq[i].rgbRed   = init_color[i][2];
	}	
	hDC = GetDC(NULL);

	bmi.bmih.biBitCount = 24 ;
	bmi.bmih.biWidth = 16;
	bmi.bmih.biHeight = 14 ;
	colbuttonBmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&colbuttonBuf, NULL, 0 ) ;
	if ( !colbuttonBmp ) {
		wsprintf( Message, TEXT("colbuttonBmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	colbuttonDC = CreateCompatibleDC( hDC ) ;
	if ( !colbuttonDC ) {
		wsprintf( Message, TEXT("colbuttonDC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( colbuttonBmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( colbuttonDC, colbuttonBmp ) ; 

	bmi.bmih.biBitCount = 24 ;
	bmi.bmih.biWidth = winWidth;
	bmi.bmih.biHeight = winHeight ;
	base24Bmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&base24Buf, NULL, 0 ) ;
	if ( !base24Bmp ) {
		wsprintf( Message, TEXT("base24Bmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	base24DC = CreateCompatibleDC( hDC ) ;
	if ( !base24DC ) {
		wsprintf( Message, TEXT("base24DC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( tmp24Bmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( base24DC, base24Bmp) ; 

	tmp24Bmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&tmp24Buf, NULL, 0 ) ;
	if ( !tmp24Bmp ) {
		wsprintf( Message, TEXT("tmp24Bmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	tmp24DC = CreateCompatibleDC( hDC ) ;
	if ( !tmp24DC ) {
		wsprintf( Message, TEXT("tmp24DC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( tmp24Bmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( tmp24DC, tmp24Bmp) ; 

	dialogBmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&dialogBuf, NULL, 0 ) ;
	if ( !tmp24Bmp ) {
		wsprintf( Message, TEXT("tmp24Bmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	dialogDC = CreateCompatibleDC( hDC ) ;
	if ( !dialogDC ) {
		wsprintf( Message, TEXT("dialogDC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( dialogBmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( dialogDC, dialogBmp) ; 

	bmi.bmih.biWidth = winWidth;
	bmi.bmih.biHeight = winHeight ;
	bmi.bmih.biBitCount = 4 ;

	baseBmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&baseBuf, NULL, 0 ) ;
	if ( !baseBmp ) {
		wsprintf( Message, TEXT("baseBmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	baseDC = CreateCompatibleDC( hDC ) ;
	if ( !baseDC ) {
		wsprintf( Message, TEXT("baseDC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( baseBmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( baseDC, baseBmp ) ; 

	tmpBmp = CreateDIBSection( hDC, (PBITMAPINFO)&bmi.bmih, DIB_RGB_COLORS,
							 (void **)&tmpBuf, NULL, 0 ) ;
	if ( !tmpBmp ) {
		wsprintf( Message, TEXT("tmpBmp error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( baseBmp);
		ReleaseDC( NULL, baseDC ) ;
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	tmpDC = CreateCompatibleDC( hDC ) ;
	if ( !tmpDC ) {
		wsprintf( Message, TEXT("tmpDC error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( baseBmp);
		ReleaseDC( NULL, baseDC ) ;
		DeleteObject( tmpBmp);
		ReleaseDC( NULL, hDC ) ;
		MessageBeep( MB_ICONASTERISK ) ;
		return  ;
	}
	SelectObject( tmpDC, tmpBmp ) ; 

	undoBuf=(BYTE *)malloc(winWidth*winHeight/2);
	if (undoBuf==NULL) {
		wsprintf( Message, TEXT("undoBuf error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		DeleteObject( baseBmp);
		ReleaseDC( NULL, baseDC ) ;
		DeleteObject( tmpBmp);
		ReleaseDC( NULL, tmpDC ) ; 
		return;
	}
		
	copyBuf=(BYTE *)malloc(winWidth*winHeight/2);
	if (copyBuf==NULL) {
		wsprintf( Message, TEXT("copyBuf error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ;
		free(undoBuf);
		DeleteObject( baseBmp);
		ReleaseDC( NULL, baseDC ) ;
		DeleteObject( tmpBmp);
		ReleaseDC( NULL, tmpDC ) ; 
		return;
	}
	freeareaBuf=(BYTE *)malloc(winWidth*winHeight/2);
	if (freeareaBuf==NULL) {
		wsprintf( Message, TEXT("copyBuf error (%d)"),GetLastError() ) ;
		MessageBox( NULL, Message, TEXT("Error"), MB_OK ) ; 
		free(copyBuf);
		free(undoBuf);
		DeleteObject( baseBmp);
		ReleaseDC( NULL, baseDC ) ;
		DeleteObject( tmpBmp);
		ReleaseDC( NULL, tmpDC ) ;   
		return;
	}

	bp_mesh=LoadBitmap(ghInst,MAKEINTRESOURCE(IDB_MESH));

	pts[0].x=-1;
	pts[0].y=-1;  
	col[0]=RGB(0,0,0);
	col[1]=RGB(0x84,0x84,0x84);
	col[2]=RGB(0xc6,0xc6,0xc6);
	col[3]=RGB(0xff,0xff,0xff);
	 
	loope_x0=-9999;
	loope_y0=-9999;

	loope_orginal_size_view_flag[0]=FALSE;
	loope_orginal_size_view_flag[1]=FALSE;
	loope_orginal_size_view_flag[2]=TRUE;
	loope_orginal_size_view_flag[3]=TRUE;
	loope_orginal_size_view_flag[4]=TRUE;

	for (j=0;j<16;j++) {
		for (i=0;i<16;i++) {
			color_mode[j][i]=TRUE;
			conv_mode[j][i]=(j==i) ? TRUE:FALSE;
		} 
		cconv[j]=j;
	}

	pat_button(hWnd,ppat);
	for (j=0;j<16;j++)
		for (i=0;i<16;i++)
			brush_pat_custum[i][j]=(i==j ? 1 : 0);

	for (j=0;j<16;j++)
		area_color[j]=TRUE;
	area_color[3]=FALSE;

	SendMessage(ghWndCB,TB_GETRECT,IDM_COLBUTTON,(LPARAM)&rc);
	colbutton_x=rc.left+4;
	colbutton_y=5;

	SetTimer(hWnd, 1000,500,NULL); 

	page_clear(hWnd);
	undo_store(hWnd);

	called_jzv_flag=FALSE;
}

/* =====================================================================
	ウィンドウ破棄時の処理
===================================================================== */

void OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HWND	hw;
	hw=FindWindow(TEXT("JZViewer"),NULL);
	if (hw!=NULL) {
		SendMessage(hw,WM_COMMAND,IDM_CALLED_JZV,0);
	}
	// コマンドバーの破棄
	CommandBar_Destroy(ghWndCB);
	PostQuitMessage(0); 
}

void pen_button(HWND hWnd)
{
	if (region_flag==PEN)
		pstyle=(pstyle+1)%3;
	region_flag=PEN;
	toolpic=TOOLPIC_PEN;
	make_command_bar(hWnd);
	menu_check();
	test_flag=TRUE;
}
BOOL col_button(HWND hWnd,int n)
{
	if (colsel_pflag==TRUE) {
		col_press_flag=TRUE;
		return FALSE;
	} else {
		pcolor=n;
		if (region_flag==ERASE) {
			region_flag=PEN;
			toolpic=TOOLPIC_PEN;
		}
		make_command_bar(hWnd);
		menu_check();
	}
	return TRUE;
}
void pat_button(HWND hWnd,int n)
{
	int i,j;
	ppat=n;
	if (region_flag==ERASE) {
		region_flag=PEN;
		toolpic=TOOLPIC_PEN;
	}
	for (j=0;j<16;j++)
		for (i=0;i<16;i++) {
			BYTE c;
			if (n<=7) {
				c=brush_pat_4[pat_inv+ppat][i%4][j%4];
			} else {
				c=brush_pat_custum[i][j];
				if (pat_inv!=0) c=1-c;
			}
			brush_pat[i][j]=c;
		}
	make_command_bar(hWnd);
	menu_check();
}
void erase_button(HWND hWnd)
{
	if (region_flag==ERASE)
		perase=(perase+1)%3;
	erase_put_flag=FALSE;
	region_flag=ERASE;
	toolpic=TOOLPIC_ERASE;
	make_command_bar(hWnd);
	menu_check();
}
void mag_button(HWND hWnd,int n)
{
	static int prev_mag=1;
	loope_num=n;
	loope_mag=lmag[loope_num];
	if (loope_mag==1) {
		if (prev_mag!=1) {
			loope_x+=winWidth/prev_mag/2;
			loope_y+=winHeight/prev_mag/2;
			loope_x0=loope_x;
			loope_y0=loope_y;
		}
		loope_x=0;
		loope_y=0;
	} else {
		if (prev_mag==1 && loope_x0>-9999) {
			loope_x=loope_x0;
			loope_y=loope_y0;
			loope_x-=winWidth/loope_mag/2;
			loope_y-=winHeight/loope_mag/2;
		} else {
			loope_x+=winWidth/prev_mag/2;
			loope_y+=winHeight/prev_mag/2;
			loope_x-=winWidth/loope_mag/2;
			loope_y-=winHeight/loope_mag/2;
		}
		loope_x0=loope_x+winWidth/loope_mag/2;
		loope_y0=loope_y+winHeight/loope_mag/2;
/*
		if (loope_x<-winWidth/loope_mag/2)				loope_x=-winWidth/loope_mag/2;
		if (loope_y<-winHeight/loope_mag/2)				loope_y=-winHeight/loope_mag/2;
		if (winWidth-winWidth/loope_mag/2<loope_x)		loope_x=winWidth-winWidth/loope_mag/2;
		if (winHeight-winHeight/loope_mag/2<loope_y)	loope_y=winHeight-winHeight/loope_mag/2;
*/
	}
	make_command_bar(hWnd);
	menu_check();
	InvalidateRect(hWnd,NULL,FALSE);
	prev_mag=loope_mag;
}

/* =====================================================================
	画面の描画
===================================================================== */
void redraw(HWND hWnd)
{
	memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
	InvalidateRect(hWnd,NULL,FALSE);
}

void paste(HWND hWnd)
{
	int x,y,x2,y2,sx,sy,ex,ey;
	int	rw,rh;

	undo_store(hWnd);
	sx=(region_sx<region_ex) ? region_sx : region_ex;
	sy=(region_sy<region_ey) ? region_sy : region_ey;
	ex=(region_sx>region_ex) ? region_sx : region_ex;
	ey=(region_sy>region_ey) ? region_sy : region_ey;
	if (sx<0) sx=0;
	if (sy<0) sy=0;
	if (ex>winWidth-1) ex=winWidth-1;
	if (ey>winHeight-1) ey=winHeight-1;
	rw=ex-sx+1;
	rh=ey-sy+1;
	for (y=0;y<=ey-sy;y++) {
		for (x=0;x<=ex-sx;x++) {
			BYTE	copy_col,org_col,c;
			switch (rotate_flag) {
				case 0: x2=x;		y2=y;		break;
				case 2: x2=rw-x;	y2=rh-y;	break;
				case 4: x2=x;		y2=rh-y;	break;
				case 6: x2=rw-x;	y2=y;		break;
				case 1: x2=rh-y;	y2=x;		break;
				case 3: x2=y;		y2=rw-x;	break;
				case 5: x2=rh-y;	y2=rw-x;	break;
				case 7: x2=y;		y2=x;		break;
			};
			if (sx+x2+select_dx<0) continue;
			if (winWidth<=sx+x2+select_dx) continue;
			if (sy+y2+select_dy<0) continue;
			if (winHeight<=sy+y2+select_dy) continue;
			org_col=get_pixel(baseBuf,sx+x2+select_dx,sy+y2+select_dy);
			copy_col=cconv[get_pixel(copyBuf,sx+x,sy+y)];
			c=color_mode[copy_col][org_col]==TRUE ? copy_col : org_col;
			if (get_pixel(freeareaBuf,sx+x,sy+y)!=0)
				set_pixel(baseBuf,sx+x2+select_dx,sy+y2+select_dy,c);
		}	
	}
	region_flag=PEN;
	toolpic=TOOLPIC_PEN;
	make_command_bar(hWnd);
	InvalidateRect(hWnd,NULL,FALSE);
}

void paste_cansel(HWND hWnd)
{
	InvalidateRect(hWnd,NULL,FALSE);
	region_flag=PEN;
	toolpic=TOOLPIC_PEN;
	make_command_bar(hWnd);
}

void circle(int sx0,int sy0,int ex0,int ey0,BYTE *buf,BOOL flag)
{
	int sx,sy,ex,ey,i,j,a,b;
	BYTE	c=pcolor;
	int		st=pstyle;
	sx=(sx0<ex0) ? sx0 : ex0;
	sy=(sy0<ey0) ? sy0 : ey0;
	ex=(sx0<ex0) ? ex0 : sx0;
	ey=(sy0<ey0) ? ey0 : sy0;
	a=(ex-sx+1)/2;
	b=(ey-sy+1)/2;
	for (j=0;j<=b;j++) {
		for (i=0;i<=a;i++) {
			if (b*b*i*i+a*a*j*j-a*a*b*b>=0) {
				if (flag==TRUE) {
					draw_line(sx+a-i,sy+b-j,sx+a+i,sy+b-j,buf,c,0,TRUE,FALSE);
					draw_line(sx+a-i,sy+b+j,sx+a+i,sy+b+j,buf,c,0,TRUE,FALSE);
				}
				put_pen(sx+a-i,sy+b-j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a-i,sy+b+j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a+i,sy+b-j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a+i,sy+b+j,buf,c,st,TRUE,FALSE);
				break;
			}
		}
	}
	for (i=0;i<=a;i++) {
		for (j=0;j<=b;j++) {
			if (b*b*i*i+a*a*j*j-a*a*b*b>=0) {
				if (flag==TRUE) {
					draw_line(sx+a-i,sy+b-j,sx+a-i,sy+b+j,buf,c,0,TRUE,FALSE);
					draw_line(sx+a+i,sy+b-j,sx+a+i,sy+b+j,buf,c,0,TRUE,FALSE);
				}
				put_pen(sx+a-i,sy+b-j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a-i,sy+b+j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a+i,sy+b-j,buf,c,st,TRUE,FALSE);
				put_pen(sx+a+i,sy+b+j,buf,c,st,TRUE,FALSE);
				break;
			}
		}
	}
}
void line_start(HWND hWnd)
{
	region_sx=-1;
	region_sy=-1;
	region_ex=0;
	region_ey=0;
	InvalidateRect(hWnd,NULL,FALSE);
//	undo_store(hWnd);
}
void line_core(BYTE *buf)
{
	int		sx,sy,ex,ey,dx,dy;
	BYTE c=pcolor;
	BYTE s=pstyle;
	sx=region_sx;
	sy=region_sy;
	ex=region_ex;
	ey=region_ey;
	dx=(sx<ex) ? ex-sx : sx-ex;
	dy=(sy<ey) ? ey-sy : sy-ey;
	if (region_flag==LINE) {
		if (action_button_flag==TRUE) {
			if (dx<dy)	ex=sx;
			else		ey=sy;
		}
		draw_line(sx,sy,ex,ey,buf,c,s,TRUE,FALSE);			
	}
	if (region_flag==RECTANGLE) {
		if (action_button_flag==TRUE) {
			if (dx<dy)	ex=sx+dy*(sx<ex ? 1:-1);
			else		ey=sy+dx*(sy<ey ? 1:-1);
		}
		if (fill_flag) {
			int i,isx,iex;
			isx=(sx<ex) ? sx : ex;
			iex=(sx>ex) ? sx : ex;
			for (i=isx;i<=iex;i++) 
				draw_line(i,sy,i,ey,buf,c,0,TRUE,FALSE);
		}
		draw_line(sx,sy,sx,ey,buf,c,s,TRUE,FALSE);			
		draw_line(sx,sy,ex,sy,buf,c,s,TRUE,FALSE);			
		draw_line(sx,ey,ex,ey,buf,c,s,TRUE,FALSE);			
		draw_line(ex,sy,ex,ey,buf,c,s,TRUE,FALSE);			
	}
	if (region_flag==CIRCLE) {
		if (action_button_flag==TRUE) {
			if (dx<dy)	ex=sx+dy*(sx<ex ? 1:-1);
			else		ey=sy+dx*(sy<ey ? 1:-1);
		}
		circle(sx,sy,ex,ey,buf,fill_flag);
	}
}
void line_end(HWND hWnd)
{
	BYTE c=pstyle;
	if (region_sx<0) return;
	line_core(baseBuf);
	line_start(hWnd);
	modified_flag=1;
}
void select_region(HWND hWnd)
{
	memcpy(copyBuf,baseBuf,winWidth*winHeight/2);
	region_sx=0;
	region_sy=0;
	region_ex=0;
	region_ey=0;
	select_x=select_dx=0;
	select_y=select_dy=0;
	select_free_x=region_sx;
	select_free_y=region_sy;
	pts[2].x=region_sx;
	pts[2].y=region_sy;
	rect_flag=TRUE;
	region_flag=SELECT;
}
void select_free(HWND hWnd)
{
	int	i;
	BYTE	*p;
	p=freeareaBuf;
	for (i=0;i<winWidth*winHeight/2;i++)
		*p++=0x33;
	memcpy(copyBuf,baseBuf,winWidth*winHeight/2);
	select_x=select_dx=0;
	select_y=select_dy=0;
	rect_flag=FALSE;
}

void select_all(HWND hWnd)
{
	memcpy(copyBuf,baseBuf,winWidth*winHeight/2);
	region_sx=0;
	region_sy=0;
	region_ex=winWidth-1;
	region_ey=winHeight-1;
	select_x=select_dx=winWidth/2;
	select_y=select_dy=winHeight/2;
	select_free_x=region_sx;
	select_free_y=region_sy;
	pts[2].x=region_sx;
	pts[2].y=region_sy;
	region_flag=SELECT;
	rect_flag=TRUE;
}
void select_magicwand(HWND hWnd)
{
	int	i;
	BYTE	*p;
	p=freeareaBuf;
	for (i=0;i<winWidth*winHeight/2;i++)
		*p++=0x33;
	memcpy(copyBuf,baseBuf,winWidth*winHeight/2);
	select_x=select_dx=0;
	select_y=select_dy=0;
	region_flag=SELECT;
	rect_flag=FALSE;
}

void cut_region(HWND hWnd)
{
	int x,y,sx,sy,ex,ey;
	
	sx=(region_sx<region_ex) ? region_sx : region_ex;
	sy=(region_sy<region_ey) ? region_sy : region_ey;
	ex=(region_sx>region_ex) ? region_sx : region_ex;
	ey=(region_sy>region_ey) ? region_sy : region_ey;
	for (y=sy;y<=ey;y++) 
		for (x=sx;x<=ex;x++) 
			if (get_pixel(freeareaBuf,x,y)!=0)
				set_pixel(baseBuf,x,y,3);
}

void free_paste_start()
{
	int x,y;
	if (start_select_rect_flag==AREA_FREE) {
		draw_line(select_free_x,select_free_y,pts[2].x,pts[2].y,freeareaBuf,1,0,FALSE,TRUE);
		for (x=0;x<winWidth;x++)
			if (get_pixel(freeareaBuf,x,0)==3)
				paint_core(freeareaBuf,x,0,0,3);
		for (x=0;x<winWidth;x++)
			if (get_pixel(freeareaBuf,x,winHeight-1)==3)
				paint_core(freeareaBuf,x,winHeight-1,0,3);
		for (y=0;y<winHeight;y++)
			if (get_pixel(freeareaBuf,0,y)==3)
				paint_core(freeareaBuf,0,y,0,3);
		for (y=0;y<winHeight;y++)
			if (get_pixel(freeareaBuf,winWidth-1,y)==3)
				paint_core(freeareaBuf,winWidth,y,0,3);
	
		for (y=1;y<winHeight-2;y++) 
			for (x=1;x<winWidth-2;x++) {
				if (get_pixel(freeareaBuf,x,y)==1) {
					if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
					if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
					if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
					if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
					set_pixel(freeareaBuf,x,y,3);
				}
			}
	} else {
		for (y=0;y<winHeight;y++) 
			for (x=0;x<winWidth;x++) {
				if (get_pixel(freeareaBuf,x,y)==1)
					continue;
				if (get_pixel(freeareaBuf,x,y)!=3) 
					set_pixel(freeareaBuf,x,y,3);
				else
					set_pixel(freeareaBuf,x,y,0);
			}
	}
}

void clipboard_inverse(HWND hWnd)
{
	int x,y;

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) 
			set_pixel(freeareaBuf,x,y,(get_pixel(freeareaBuf,x,y)==0) ? 1 : 0); 

	for (y=1;y<winHeight-2;y++)
		for (x=1;x<winWidth-2;x++) {
			if (get_pixel(freeareaBuf,x,y)==1) {
				if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
				if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
				set_pixel(freeareaBuf,x,y,3);
			}
		}
	rect_flag=FALSE;
	clipboard_crop(freeareaBuf,0);
}

void clipboard_lasso(HWND hWnd)
{ 
	int x,y;

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) 
			if (get_pixel(freeareaBuf,x,y)!=0) {
				set_pixel(freeareaBuf,x,y,1);
				if (area_color[get_pixel(copyBuf,x,y)]==FALSE)
					set_pixel(freeareaBuf,x,y,0);
			}

	for (x=0;x<winWidth;x++) 
		if (get_pixel(freeareaBuf,x,0)==0)
			paint_core(freeareaBuf,x,0,2,0);
	for (x=0;x<winWidth;x++)
		if (get_pixel(freeareaBuf,x,winHeight-1)==0)
			paint_core(freeareaBuf,x,winHeight-1,2,0);
	for (y=0;y<winHeight;y++)
		if (get_pixel(freeareaBuf,0,y)==0)
			paint_core(freeareaBuf,0,y,2,0);
	for (y=0;y<winHeight;y++)
		if (get_pixel(freeareaBuf,winWidth-1,y)==0)
			paint_core(freeareaBuf,winWidth,y,2,0);

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) {
			if (get_pixel(freeareaBuf,x,y)==0)
				set_pixel(freeareaBuf,x,y,1);
			if (get_pixel(freeareaBuf,x,y)==2)
				set_pixel(freeareaBuf,x,y,0);
		}

	for (y=1;y<winHeight-2;y++)
		for (x=1;x<winWidth-2;x++) {
			if (get_pixel(freeareaBuf,x,y)==1) {
				if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
				if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
				set_pixel(freeareaBuf,x,y,3);
			}
		}
	rect_flag=FALSE;
	clipboard_crop(freeareaBuf,0);
}

void clipboard_magicwand(HWND hWnd)
{
	int x,y;

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) 
			if (get_pixel(freeareaBuf,x,y)!=0) {
				set_pixel(freeareaBuf,x,y,1);
				if (area_color[get_pixel(copyBuf,x,y)]==FALSE)
					set_pixel(freeareaBuf,x,y,0);
			}

	for (y=1;y<winHeight-2;y++)
		for (x=1;x<winWidth-2;x++) {
			if (get_pixel(freeareaBuf,x,y)==1) {
				if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
				if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
				set_pixel(freeareaBuf,x,y,3);
			}
		}
	rect_flag=FALSE;
	clipboard_crop(freeareaBuf,0);
}

void clipboard_bold(HWND hWnd)
{
	int x,y;

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) 
			if (get_pixel(freeareaBuf,x,y)!=0)  
				set_pixel(freeareaBuf,x,y,1);

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) {
			BOOL flag=FALSE;
			if (get_pixel(freeareaBuf,x,y)==0) {
				if (x!=0           && get_pixel(freeareaBuf,x-1,y  )==1) flag=TRUE;
				if (y!=0           && get_pixel(freeareaBuf,x  ,y-1)==1) flag=TRUE;
				if (x!=winWidth-1  && get_pixel(freeareaBuf,x+1,y  )==1) flag=TRUE;
				if (y!=winHeight-1 && get_pixel(freeareaBuf,x  ,y+1)==1) flag=TRUE;
				if (flag==TRUE) set_pixel(freeareaBuf,x,y,3);
			}
		}
	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) 
			if (get_pixel(freeareaBuf,x,y)!=0)
				set_pixel(freeareaBuf,x,y,1);
	for (y=1;y<winHeight-2;y++)
		for (x=1;x<winWidth-2;x++) {
			if (get_pixel(freeareaBuf,x,y)==1) {
				if (get_pixel(freeareaBuf,x-1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x+1,y  )==0) continue;
				if (get_pixel(freeareaBuf,x  ,y-1)==0) continue;
				if (get_pixel(freeareaBuf,x  ,y+1)==0) continue;
				set_pixel(freeareaBuf,x,y,3);
			}
		}
	rect_flag=FALSE;
	clipboard_crop(freeareaBuf,0);
}

void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC         hDC;
	RECT        rc;
	int			ex_flag=0;
	HPEN		hPen;
	HBRUSH		hBrush;
	HGDIOBJ		hOldPen,hOldBrush;

	if (GetActiveWindow()==hWnd) {
		active_flag=TRUE;
	}
		
	hDC = BeginPaint(hWnd, &ps);
	GetWindowRect(hWnd, &rc);
	// コマンドバーの部分を除く
	rc.top += cbh;
	//	BitBlt(hDC, 0, rc.top, winWidth, winHeight,baseDC, 0, 0, SRCCOPY); 
	if (col_menu_flag==TRUE) {
		SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_COLBUTTON,MAKELONG(FALSE, 0));
		col_menu_flag=FALSE;
	}
	if (pat_menu_flag==TRUE) {
		SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_PATBUTTON,MAKELONG(FALSE, 0));
		pat_menu_flag=FALSE;
	}
	if (loope_menu_flag==TRUE) {
		SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_MAGBUTTON,MAKELONG(FALSE, 0));
		loope_menu_flag=FALSE;
	}
	if (tool_menu_flag==TRUE) {
		SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_TOOLBUTTON,MAKELONG(FALSE, 0));
		tool_menu_flag=FALSE;
	}

	if (color_set_mode!=COLOR_SET_NONE || color_conv_mode!=COLOR_CONV_NONE || color_mask_mode!=COLOR_CONV_NONE || 
		color_bg_mode!=COLOR_BG_NONE || color_area_mode!=COLOR_AREA_NONE || color_load_mode!=COLOR_LOAD_NONE) {
		if (loope_mag==1) {
			if (loope_x!=0 || loope_y!=0) {
				hPen=CreatePen(PS_NULL,0,0);
				hOldPen=SelectObject(tmp24DC,hPen);
				hBrush=CreatePatternBrush(bp_mesh);	
				hOldBrush=SelectObject(tmp24DC,hBrush);
				if (loope_x<0)	Rectangle(tmp24DC,0,rc.top-cbh,-loope_x,rc.bottom);		
				if (loope_y<0)	Rectangle(tmp24DC,rc.left,0,rc.right,-loope_y+cbh);		
				if (loope_x>0)	Rectangle(tmp24DC,winWidth-loope_x,rc.top-cbh,rc.right,rc.bottom);		
				if (loope_y>0)	Rectangle(tmp24DC,rc.left,winHeight-loope_y,rc.right,rc.bottom);		
				DeleteObject(SelectObject(tmp24DC,hOldBrush));
				DeleteObject(SelectObject(tmp24DC,hOldPen));
			}
			BitBlt(tmp24DC, -loope_x, -loope_y, winWidth, winHeight, base24DC, 0,0, SRCCOPY);
		} else {
			int		x1,y1,x2,y2;
			if ((loope_x<0) || 
				(loope_y<0) || 
				(loope_x>winWidth-winWidth/loope_mag) || 
				(loope_y>winHeight-winHeight/loope_mag)) {
				ex_flag=1;
			}
			if (ex_flag) {
				hPen=CreatePen(PS_NULL,0,0);
				hOldPen=SelectObject(tmp24DC,hPen);
				hBrush=CreatePatternBrush(bp_mesh);	
				hOldBrush=SelectObject(tmp24DC,hBrush); 
			}
			for (x1=0;x1<winWidth;x1++)
				if (x1*loope_mag>winWidth/loope_mag-1) break;
			for (y1=0;y1<winHeight;y1++)
				if (y1*loope_mag>winHeight/loope_mag-1) break;
			for (x2=0;x2<winWidth;x2++)
				if (x2*loope_mag>winWidth) break;
			for (y2=0;y2<winHeight;y2++)
				if (y2*loope_mag>winHeight) break;
							
			if (!loope_orginal_size_view_flag[loope_num]) {
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(tmp24DC,0,0,-loope_x*loope_mag,rc.bottom);		
					if (loope_y<0)
						Rectangle(tmp24DC,0,0,rc.right,-loope_y*loope_mag);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(tmp24DC,(winWidth-loope_x)*loope_mag,rc.top,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(tmp24DC,rc.left,(winHeight-loope_y)*loope_mag,rc.right,rc.bottom);		
				}
				StretchBlt(tmp24DC, 0, rc.top, x2*loope_mag, y2*loope_mag,base24DC, loope_x, loope_y, x2,y2, SRCCOPY);
			} else {
				int i,j;
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(tmp24DC,0,y2,-loope_x*loope_mag,rc.bottom);		
					if (loope_x<0 && -loope_x>x1) 
						Rectangle(tmp24DC,x1*loope_mag,0,-loope_x*loope_mag,y2);		
					if (loope_y<0)
						Rectangle(tmp24DC,x1*loope_mag,0,rc.right,-loope_y*loope_mag);		
					if (loope_y<0 && -loope_y*loope_mag>y2) 
						Rectangle(tmp24DC,0,y2,x1*loope_mag,-loope_y*loope_mag);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(tmp24DC,(winWidth-loope_x)*loope_mag,rc.top,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(tmp24DC,rc.left,(winHeight-loope_y)*loope_mag,rc.right,rc.bottom);		
				}
				StretchBlt(tmp24DC, x1*loope_mag, rc.top-cbh, (x2-x1)*loope_mag, y1*loope_mag,
					base24DC, loope_x+x1, loope_y, x2-x1,y1, SRCCOPY);
				StretchBlt(tmp24DC, 0, rc.top-cbh+(y1)*loope_mag, x2*loope_mag, (y2-y1+1)*loope_mag,
					base24DC, loope_x, loope_y+y1, x2,y2-y1+1, SRCCOPY);
				if (ex_flag)
					Rectangle(tmp24DC,0,0,x1*loope_mag-2,y2-2);		
				BitBlt(tmp24DC, 0, 0, x1*loope_mag, y2,	base24DC, loope_x, loope_y, SRCCOPY);
				
				for (j=0;j<y2;j++)
					for (i=x1*loope_mag-2;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);
				for (j=y2-2;j<y2;j++)
					for (i=0;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);
			}
			if (ex_flag) {
				DeleteObject(SelectObject(hDC,hOldBrush));
				DeleteObject(SelectObject(hDC,hOldPen));
			}
		}	
		if (color_load_mode!=COLOR_LOAD_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, pspc_flag ? 120:190, pspc_flag ? 125:190, dialogDC, 0,0, SRCCOPY);
		if (color_set_mode!=COLOR_SET_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, pspc_flag ? 120:190, pspc_flag ? 110:135, dialogDC, 0,0, SRCCOPY);
		if (color_conv_mode!=COLOR_CONV_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, 190, 190, dialogDC, 0,0, SRCCOPY);
		if (color_mask_mode!=COLOR_MODE_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, 190, 190, dialogDC, 0,0, SRCCOPY);
		if (color_bg_mode!=COLOR_BG_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, 190, 90, dialogDC, 0,0, SRCCOPY);
		if (color_area_mode!=COLOR_AREA_NONE)
			BitBlt(tmp24DC, colset_x,colset_y, 190, 70, dialogDC, 0,0, SRCCOPY);
		BitBlt(hDC, 0,cbh, winWidth, winHeight, tmp24DC, 0,0, SRCCOPY);
		if (colload_state==COLLOAD_MOVE2) colload_state=COLLOAD_NONE;
		if (colset_state==COLSET_MOVE2) colset_state=COLSET_NONE;
		if (colconv_state==COLCONV_MOVE2) colconv_state=COLCONV_NONE;
		if (colmode_state==COLMODE_MOVE2) colmode_state=COLMODE_NONE;
		if (colbg_state==COLBG_MOVE2) colbg_state=COLBG_NONE;
		if (colarea_state==COLAREA_MOVE2) colarea_state=COLAREA_NONE;
		EndPaint(hWnd, &ps);
		return;
	}
	if (loope_mag==1) {
		if (region_flag==SELECT || region_flag==SELECT_END ||
			region_flag==SELECT_FREE || region_flag==SELECT_FREE_END || region_flag==DRAG) {
			int		px,py,sx,sy,ex,ey,sx2,sy2,ex2,ey2;
			int		len=0;
			memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
			memcpy(tmp24Buf,base24Buf,winWidth*winHeight*3);
			sx=sx2=(region_sx<region_ex) ? region_sx : region_ex;
			sy=sy2=(region_sy<region_ey) ? region_sy : region_ey;
			ex=ex2=(region_sx>region_ex) ? region_sx : region_ex;
			ey=ey2=(region_sy>region_ey) ? region_sy : region_ey;
			if (region_flag==DRAG) {
				int x,y,x2,y2;
				int rw=ex-sx+1,rh=ey-sy+1;
				if (sx<0) sx=0;
				if (sy<0) sy=0;
				if (ex>winWidth-1) ex=winWidth-1;
				if (ey>winHeight-1) ey=winHeight-1;
				for (y=0;y<=ey-sy;y++) {
					for (x=0;x<=ex-sx;x++) {
						BYTE	copy_col,org_col,c;
						if (get_pixel(freeareaBuf,x+sx,y+sy)==0) continue;
						switch (rotate_flag) {
							case 0: x2=x;		y2=y;		break;
							case 2: x2=rw-x;	y2=rh-y;	break;
							case 4: x2=x;		y2=rh-y;	break;
							case 6: x2=rw-x;	y2=y;		break;
							case 1: x2=rh-y;	y2=x;		break;
							case 3: x2=y;		y2=rw-x;	break;
							case 5: x2=rh-y;	y2=rw-x;	break;
							case 7: x2=y;		y2=x;		break;
						};
						org_col=get_pixel(baseBuf,x2+sx+select_dx,y2+sy+select_dy);
						copy_col=cconv[get_pixel(copyBuf,x+sx,y+sy)];
						c=color_mode[copy_col][org_col]==TRUE ? copy_col : org_col;
						set_pixel_24(tmp24Buf,sx+x2+select_dx,sy+y2+select_dy,c);
					}
				}
				if ((rotate_flag%2)==1 && rect_flag!=FALSE) {
					ex=ex2=sx+rh;
					ey=ey2=sy+rw;
				}
				sx2=sx+select_dx;
				ex2=ex+select_dx;
				sy2=sy+select_dy;
				ey2=ey+select_dy;
			}
			if (rect_flag==FALSE) {
				if (region_flag!=DRAG) {
					BYTE	c;
					for (py=sy;py<=ey;py++)
						for (px=sx;px<=ex;px++) {
							c=get_pixel(freeareaBuf,px,py);
							if (c!=1) continue;
							set_pixel_24(tmp24Buf,px,py,(((px+py)/4)%2)==(tm%2) ? 16+0 : 16+3);
						}
					if (start_select_rect_flag==AREA_FREE) {
						force_24_flag=TRUE;
						draw_line(select_free_x,select_free_y,pts[2].x,pts[2].y,tmp24Buf,(tm%2)==1 ? 16+0 : 16+3,0,FALSE,TRUE);
						force_24_flag=FALSE;
					}
				} else {
					int rw=ex-sx+1,rh=ey-sy+1;
					BYTE	c;
					int x,y,x2,y2;
					for (y=0;y<=ey-sy;y++)
						for (x=0;x<=ex-sx;x++) {
							c=get_pixel(freeareaBuf,sx+x,sy+y);
							if (c!=1) continue;
							switch (rotate_flag) {
								case 0: x2=x;		y2=y;		break;
								case 2: x2=rw-x;	y2=rh-y;	break;
								case 4: x2=x;		y2=rh-y;	break;
								case 6: x2=rw-x;	y2=y;		break;
								case 1: x2=rh-y;	y2=x;		break;
								case 3: x2=y;		y2=rw-x;	break;
								case 5: x2=rh-y;	y2=rw-x;	break;
								case 7: x2=y;		y2=x;		break;
							};
							if (sx+x2+select_dx<0) continue;
							if (winWidth<=sx+x2+select_dx) continue;
							if (sy+y2+select_dy<0) continue;
							if (winHeight<=sy+y2+select_dy) continue;
							set_pixel_24(tmp24Buf,sx+x2+select_dx,sy+y2+select_dy,(((x+y)/4)%2)==(tm%2) ? 16+0 : 16+3);
						}
				}
			} else {
				if ((tm&1)==0)	len=0;
				else			len=3;
				for (px=sx2;px<=ex2;px++,len++) 
					set_pixel_24(tmp24Buf,px,sy2,(len%6)<3 ? 16+0 : 16+3);
				for (py=sy2;py<=ey2;py++,len++) 
					set_pixel_24(tmp24Buf,ex2,py,(len%6)<3 ? 16+0 : 16+3);
				for (px=ex2;px>=sx2;px--,len++) 
					set_pixel_24(tmp24Buf,px,ey2,(len%6)<3 ? 16+0 : 16+3);
				for (py=ey2;py>=sy2;py--,len++) 
					set_pixel_24(tmp24Buf,sx2,py,(len%6)<3 ? 16+0 : 16+3);
			}
		}

		if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
			if (region_sx>=0) {
				memcpy(tmp24Buf,base24Buf,winWidth*winHeight*3);
				force_24_flag=TRUE;
				line_core(tmp24Buf);
				force_24_flag=FALSE;
			}
		}

		if (loope_x!=0 || loope_y!=0) {
			hPen=CreatePen(PS_NULL,0,0);
			hOldPen=SelectObject(hDC,hPen);
			hBrush=CreatePatternBrush(bp_mesh);	
			hOldBrush=SelectObject(hDC,hBrush);
			if (loope_x<0)	Rectangle(hDC,0,rc.top-cbh,-loope_x,rc.bottom);		
			if (loope_y<0)	Rectangle(hDC,rc.left,0,rc.right,-loope_y+cbh);		
			if (loope_x>0)	Rectangle(hDC,winWidth-loope_x,rc.top-cbh,rc.right,rc.bottom);		
			if (loope_y>0)	Rectangle(hDC,rc.left,winHeight-loope_y+cbh,rc.right,rc.bottom);		
			DeleteObject(SelectObject(hDC,hOldBrush));
			DeleteObject(SelectObject(hDC,hOldPen));
		}
		if (region_flag==SELECT || region_flag==SELECT_END ||
			region_flag==SELECT_FREE || region_flag==SELECT_FREE_END || region_flag==DRAG ||
			((region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) && region_sx>=0)) {
			BitBlt(hDC, -loope_x, -loope_y+rc.top, winWidth, winHeight, tmp24DC, 0,0, SRCCOPY);
		} else {
			BitBlt(hDC, rc.left-loope_x, rc.top-loope_y, rc.right-rc.left, rc.bottom-rc.top, base24DC, 0,0, SRCCOPY);
		}
		if (erase_put_flag) {
			int		p=style[1][perase];
			int t=tm%2;
			int i,j;
			for (j=0;j<p;j++)
				for (i=0;i<p;i++)
					set_pixel(tmpBuf,i,j,3);
			if (p!=1) {
				for (i=-p/2;i<-p/2+p;i++) {
					set_pixel_24(tmpBuf,i,0,(i/2)%2==t ? 16+0 : 16+3);
					set_pixel_24(tmpBuf,0,i,(i/2)%2==t ? 16+0 : 16+3);
					set_pixel_24(tmpBuf,i,p-1,(i/2)%2==t ? 16+3 : 16+0);
					set_pixel_24(tmpBuf,p-1,i,(i/2)%2==t ? 16+3 : 16+0);
				}
			}
			BitBlt(hDC, -loope_x+erase_x,-loope_y+erase_y+rc.top,
				p, p, tmp24DC, 0,0, SRCCOPY);
		}
	} else {
		int		x1,y1,x2,y2;

		if ((loope_x<0) || 
			(loope_y<0) || 
			(loope_x>winWidth-winWidth/loope_mag) || 
			(loope_y>winHeight-winHeight/loope_mag)) {
				ex_flag=1;
		}
		for (x1=0;x1<winWidth;x1++)
			if (x1*loope_mag>winWidth/loope_mag-1) break;
		for (y1=0;y1<winHeight;y1++)
			if (y1*loope_mag>winHeight/loope_mag-1) break;
		for (x2=0;x2<winWidth;x2++)
			if (x2*loope_mag>winWidth) break;
		for (y2=0;y2<winHeight;y2++)
			if (y2*loope_mag>winHeight) break;
		if (region_flag==SELECT || region_flag==SELECT_END || 
			region_flag==SELECT_FREE || region_flag==SELECT_FREE_END || region_flag==DRAG) {			
			int		px,py,sx,sy,ex,ey,ex2,ey2;
			int		len=0,i,j;

			if (ex_flag) {
				hPen=CreatePen(PS_NULL,0,0);
				hOldPen=SelectObject(tmp24DC,hPen);
				hBrush=CreatePatternBrush(bp_mesh);	
				hOldBrush=SelectObject(tmp24DC,hBrush);
			}

			if (loope_orginal_size_view_flag[loope_num]) {
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(tmp24DC,0,y2,-loope_x*loope_mag,rc.bottom);		
					if (loope_x<0 && -loope_x>x1) 
						Rectangle(tmp24DC,x1*loope_mag,0,-loope_x*loope_mag,y2);		
					if (loope_y<0)
						Rectangle(tmp24DC,x1*loope_mag,0,rc.right,-loope_y*loope_mag);		
					if (loope_y<0 && -loope_y*loope_mag>y2) 
						Rectangle(tmp24DC,0,y2,x1*loope_mag,-loope_y*loope_mag);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(tmp24DC,(winWidth-loope_x)*loope_mag,rc.top-cbh,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(tmp24DC,rc.left,(winHeight-loope_y)*loope_mag,rc.right,rc.bottom);		
				}
				StretchBlt(tmp24DC, x1*loope_mag, 0, (x2-x1)*loope_mag, y1*loope_mag,
						base24DC, loope_x+x1, loope_y, x2-x1,y1, SRCCOPY);
				StretchBlt(tmp24DC, 0,(y1-1)*loope_mag, x2*loope_mag, (y2-y1+1)*loope_mag,
						base24DC, loope_x, loope_y+y1-1, x2,y2-y1+1, SRCCOPY);
			} else {
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(tmp24DC,0,0,-loope_x*loope_mag,rc.bottom);		
					if (loope_y<0)
						Rectangle(tmp24DC,0,0,rc.right,-loope_y*loope_mag);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(tmp24DC,(winWidth-loope_x)*loope_mag,rc.top-cbh,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(tmp24DC,rc.left,(winHeight-loope_y)*loope_mag,rc.right,rc.bottom);		
				}
				StretchBlt(tmp24DC, 0, 0, x2*loope_mag, y2*loope_mag,
						base24DC, loope_x, loope_y, x2,y2, SRCCOPY);
			}

			sx=(region_sx<region_ex) ? region_sx : region_ex;
			sy=(region_sy<region_ey) ? region_sy : region_ey;
			ex=(region_sx>region_ex) ? region_sx : region_ex;
			ey=(region_sy>region_ey) ? region_sy : region_ey;
			if (sx<0) sx=0;
			if (sy<0) sy=0;
			if (ex>winWidth-1) ex=winWidth-1;
			if (ey>winHeight-1) ey=winHeight-1;
			if (region_flag==DRAG) {
				int x,y,i,j;
				int		x2,y2,rw=ex-sx+1,rh=ey-sy+1;
				for (y=0;y<=ey-sy;y++) {
					for (x=0;x<=ex-sx;x++) {
						BYTE	copy_col,org_col,c;
						if (get_pixel(freeareaBuf,sx+x,sy+y)==0) continue;
						switch (rotate_flag) {
							case 0: x2=x;		y2=y;		break;
							case 2: x2=rw-x;	y2=rh-y;	break;
							case 4: x2=x;		y2=rh-y;	break;
							case 6: x2=rw-x;	y2=y;		break;
							case 1: x2=rh-y;	y2=x;		break;
							case 3: x2=y;		y2=rw-x;	break;
							case 5: x2=rh-y;	y2=rw-x;	break;
							case 7: x2=y;		y2=x;		break;
						}
						if (sx+x2+select_dx<0) continue;
						if (winWidth<=sx+x2+select_dx) continue;
						if (sy+y2+select_dy<0) continue;
						if (winHeight<=sy+y2+select_dy) continue;
						org_col=get_pixel(baseBuf,sx+x2+select_dx,sy+y2+select_dy);
						copy_col=cconv[get_pixel(copyBuf,sx+x,sy+y)];
						c=color_mode[copy_col][org_col]==TRUE ? copy_col : org_col;
						for (j=0;j<loope_mag;j++)
							for (i=0;i<loope_mag;i++)
								set_pixel_24(tmp24Buf,
									(-loope_x+sx+x2+select_dx)*loope_mag+i,
									(-loope_y+sy+y2+select_dy)*loope_mag+j,c);
					}
				}
				if ((rotate_flag%2)==1 && rect_flag!=FALSE) {
					ex=ex2=sx+rh;
					ey=ey2=sy+rw;
				}
			}
			if (rect_flag==FALSE) {
				if (region_flag!=DRAG) {
					int i,j;
					BYTE	c,cc0,cc1;
					for (py=sy;py<=ey;py++) {
						if (py<loope_y) continue;
						if (py>=winHeight/loope_mag+loope_y) continue;
						for (px=sx;px<=ex;px++) {
							if (px<loope_x) continue;
							if (px>=winWidth/loope_mag+loope_x) continue;
							c=get_pixel(freeareaBuf,px,py);
							if (c!=1) continue;
							cc0=(((px+py)/4)%2)==(tm%2) ? 0 : 3;
							cc1=(((px+py)/4)%2)==(tm%2) ? 3 : 0;
							for (j=0;j<loope_mag;j++)
								for (i=0;i<loope_mag;i++) {
									if (i==0 || j==0 || i==loope_mag-1 || j==loope_mag-1) {
										set_pixel_24(tmp24Buf,(px-loope_x)*loope_mag+i,
											(py-loope_y)*loope_mag+j,(i+j)%2==0 ? 16+cc0 : 16+cc1);
									}
								}
						}
					}
					if (start_select_rect_flag==AREA_FREE) {
						force_24_flag=TRUE;
						draw_line((select_free_x-loope_x)*loope_mag,(select_free_y-loope_y)*loope_mag,
									(pts[2].x-loope_x)*loope_mag,(pts[2].y-loope_y)*loope_mag,
									tmp24Buf,(tm%2)==1 ? 16+0 : 16+3,0,FALSE,TRUE);
						force_24_flag=FALSE;
					}
				} else {
					int i,x,y;
					for (py=sy;py<=ey;py++) {
//						if (py+select_dy<loope_y) continue;
//						if (py+select_dy>=winHeight/loope_mag+loope_y) continue;
						for (px=sx;px<=ex;px++) {
//							if (px+select_dx<loope_x) continue;
//							if (px+select_dx>=winWidth/loope_mag+loope_x) continue;
							if (get_pixel(freeareaBuf,px,py)==0) continue;
							x=(px-loope_x+select_dx)*loope_mag;
							y=(py-loope_y+select_dy)*loope_mag;
							if (get_pixel(freeareaBuf,px-1,py)==0) 
								for (i=0;i<loope_mag;i++)
									set_pixel_24(tmp24Buf,x,y+i,(i+tm)%2 ? 16+0 : 16+3);
							if (get_pixel(freeareaBuf,px+1,py)==0) 
								for (i=0;i<loope_mag;i++)
									set_pixel_24(tmp24Buf,x+loope_mag-1,y+i,(i+tm)%2 ? 16+0 : 16+3);
							if (get_pixel(freeareaBuf,px,py-1)==0) 
								for (i=0;i<loope_mag;i++)
									set_pixel_24(tmp24Buf,x+i,y,(i+tm)%2 ? 16+0 : 16+3);
							if (get_pixel(freeareaBuf,px,py+1)==0) 
								for (i=0;i<loope_mag;i++)
									set_pixel_24(tmp24Buf,x+i,y+loope_mag-1,(i+tm)%2 ? 16+0 : 16+3);
						}
					}
				}
			} else {
				if (region_flag==DRAG) {
					sx+=select_dx;
					ex+=select_dx;
					sy+=select_dy;
					ey+=select_dy;
				}
				sx=(-loope_x+sx)*loope_mag;
				ex=(-loope_x+ex+1)*loope_mag;
				sy=(-loope_y+sy)*loope_mag;
				ey=(-loope_y+ey+1)*loope_mag;
				if ((tm&1)==0)	len=0;
				else			len=3;
				for (px=sx;px<=ex;px++,len++) 
					set_pixel_24(tmp24Buf,px,sy,(len%6)<3 ? 16+0 : 16+3);
				for (py=sy;py<=ey;py++,len++) 
					set_pixel_24(tmp24Buf,ex-1,py,(len%6)<3 ? 16+0 : 16+3);
				for (px=ex-1;px>=sx;px--,len++) 
					set_pixel_24(tmp24Buf,px,ey,(len%6)<3 ? 16+0 : 16+3);
				for (py=ey;py>=sy;py--,len++) 
					set_pixel_24(tmp24Buf,sx,py,(len%6)<3 ? 16+0 : 16+3);
			}
			if (loope_orginal_size_view_flag[loope_num]) {
				if (ex_flag) {
					Rectangle(tmp24DC,0,0,winWidth/loope_mag,winHeight/loope_mag);		
				}
				BitBlt(tmp24DC, 0, 0, x1*loope_mag, y2,
						base24DC, loope_x, loope_y, SRCCOPY);
			}
			if (loope_orginal_size_view_flag[loope_num]) {
				for (j=0;j<y2;j++)
					for (i=x1*loope_mag-2;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);
				for (j=y2-2;j<y2;j++)
					for (i=0;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);				
			}

			BitBlt(hDC, 0, rc.top, winWidth, winHeight,
					tmp24DC, 0, 0, SRCCOPY);

			if (ex_flag) {
				DeleteObject(SelectObject(tmp24DC,hOldBrush));
				DeleteObject(SelectObject(tmp24DC,hOldPen)); 
			}
		} else {
			int i,j;
			memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
			memcpy(tmp24Buf,base24Buf,winWidth*winHeight*3);
			if (ex_flag) {
					hPen=CreatePen(PS_NULL,0,0);
					hOldPen=SelectObject(hDC,hPen);
					hBrush=CreatePatternBrush(bp_mesh);	
					hOldBrush=SelectObject(hDC,hBrush); 
			}
			if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
				if (region_sx>=0) {
					force_24_flag=TRUE;
					line_core(tmp24Buf);
					force_24_flag=FALSE;
				}
			}
			if (!loope_orginal_size_view_flag[loope_num]) {
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(hDC,0,0+cbh,-loope_x*loope_mag,rc.bottom);		
					if (loope_y<0)
						Rectangle(hDC,0,0,rc.right,-loope_y*loope_mag+cbh);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(hDC,(winWidth-loope_x)*loope_mag,rc.top-cbh,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(hDC,rc.left,(winHeight-loope_y)*loope_mag+cbh,rc.right,rc.bottom);		
				}
				StretchBlt(hDC, 0, rc.top, x2*loope_mag, y2*loope_mag,
						tmp24DC, loope_x, loope_y, x2,y2, SRCCOPY);
			} else {
				if (ex_flag) {
					if (loope_x<0)	
						Rectangle(hDC,0,y2+cbh,-loope_x*loope_mag,rc.bottom);		
					if (loope_x<0 && -loope_x>x1) 
						Rectangle(hDC,x1*loope_mag,cbh,-loope_x*loope_mag,y2+cbh);		
					if (loope_y<0)
						Rectangle(hDC,x1*loope_mag,0,rc.right,-loope_y*loope_mag+cbh);		
					if (loope_y<0 && -loope_y*loope_mag>y2) 
						Rectangle(hDC,0,y2+cbh,x1*loope_mag,-loope_y*loope_mag+cbh);		
					if ((winWidth-loope_x)*loope_mag<winWidth)	
						Rectangle(hDC,(winWidth-loope_x)*loope_mag,rc.top-cbh,winWidth,rc.bottom);		
					if ((winHeight-loope_y)*loope_mag<winHeight)	
						Rectangle(hDC,rc.left,(winHeight-loope_y)*loope_mag+cbh,rc.right,rc.bottom);		
				}
				StretchBlt(hDC, x1*loope_mag, rc.top, (x2-x1)*loope_mag, y1*loope_mag,
						tmp24DC, loope_x+x1, loope_y, x2-x1,y1, SRCCOPY);
				StretchBlt(hDC, 0, rc.top+(y1)*loope_mag, x2*loope_mag, (y2-y1+1)*loope_mag,
						tmp24DC, loope_x, loope_y+y1, x2,y2-y1+1, SRCCOPY);
			}
			if (loope_orginal_size_view_flag[loope_num]) {
				if (ex_flag) {
					Rectangle(hDC,0,cbh,x1*loope_mag-2,y2-2+cbh);		
				}
				BitBlt(hDC, 0, cbh, x1*loope_mag, y2,	tmp24DC, loope_x, loope_y, SRCCOPY);
			}

			if (erase_put_flag) {
				int i,j,p=style[1][perase];
				int px,py;
				px=(erase_x-loope_x)*loope_mag;
				py=(erase_y-loope_y)*loope_mag;
				for (j=0;j<p*loope_mag;j++)
					for (i=0;i<p*loope_mag;i++)
					set_pixel(tmpBuf,px+i,py+j,3);
				for (i=0;i<p*loope_mag;i++) {
					set_pixel_24(tmp24Buf,px+i,py,(i/2)%2==tm%2 ? 16+3:16+0);
					set_pixel_24(tmp24Buf,px,py+i,(i/2)%2==tm%2 ? 16+3:16+0);
					set_pixel_24(tmp24Buf,px+i,py+p*loope_mag-1,(i/2)%2==tm%2 ? 16+3:16+0);
					set_pixel_24(tmp24Buf,px+p*loope_mag-1,py+i,(i/2)%2==tm%2 ? 16+3:16+0);
				}
				BitBlt(hDC, px,py+cbh, p*loope_mag,p*loope_mag,
						tmp24DC, px,py, SRCCOPY);
			}
			if (loope_orginal_size_view_flag[loope_num]) {
				for (j=0;j<y2;j++)
					for (i=x1*loope_mag-2;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);
				BitBlt(hDC, x1*loope_mag-2, rc.top, 2, y2,
						tmp24DC, x1*loope_mag-2, 0, SRCCOPY);
				for (j=y2-2;j<y2;j++)
					for (i=0;i<x1*loope_mag;i++)
						set_pixel_24(tmp24Buf,i,j,(i+j)%2 ? 16+0 : 16+3);
				BitBlt(hDC, 0, y2-2+cbh, x1*loope_mag, 2,
						tmp24DC, 0, y2-2, SRCCOPY);
			}

			if (ex_flag) {
				DeleteObject(SelectObject(hDC,hOldBrush));
				DeleteObject(SelectObject(hDC,hOldPen));
			}
		}	
	}
	EndPaint(hWnd, &ps);
/*
	if (col_press_flag==TRUE) {
		col_press_flag=FALSE;
		SendMessage(ghWndCB, TB_PRESSBUTTON, IDM_COLBUTTON,MAKELONG(TRUE, 0));
							SendMessage(lpnmtb->hdr.hwndFrom,TB_GETRECT,(WPARAM)lpnmtb->iItem,(LPARAM)&rc);
							MapWindowPoints(lpnmtb->hdr.hwndFrom,HWND_DESKTOP,(LPPOINT)&rc,2);
							tpm.cbSize=sizeof(TPMPARAMS);	
							tpm.rcExclude.top=rc.top;	
							tpm.rcExclude.left=rc.left;
							tpm.rcExclude.bottom=rc.bottom;	
							tpm.rcExclude.right=rc.right;	
							col_pulldown_x_pos=rc.left+30;
							TrackPopupMenuEx(col_menu,TPM_LEFTALIGN|TPM_VERTICAL,rc.left,rc.bottom,hWnd,&tpm);
							col_menu_flag=TRUE;
	}
*/
}

void mouse_down(HWND hWnd, WPARAM wParam, LPARAM lParam)
{ 
	int			x,y; 

	mouse_flag=MOVE;
	line_adjust=0;
	SetCapture(hWnd);
//	undo_store(hWnd);
	x = (LOWORD(lParam))/loope_mag+loope_x;
	y = (HIWORD(lParam)-cbh)/loope_mag+loope_y;

	if (color_load_mode==COLOR_LOAD_START) {
		int		i,j,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (pspc_flag) {
			if (colset_x<=xx && xx<=120+colset_x && colset_y<=yy && yy<=125+colset_y) {
				xx-=colset_x;
				yy-=colset_y;
				colset_state=COLSET_NONE;
				color_load_open_flag=FALSE;
				colset_exchange=-1;
				tmp_col_sel=-1;

				if (xx<120-40 && yy<17) {
					colload_state=COLLOAD_MOVE;
					colset_move_x=xx;
					colset_move_y=yy;
				}

				if (25<=yy && yy<53) {
					i=(xx-10)/13;
					j=(yy-25)/13;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=j*8+i;
					colset_exchange=tmp_col_sel;
					put_select_color(tmp_col_sel,-1);
					put_load_color(-1);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (53<=yy && yy<65+25+10) {
					i=(xx-10)/13;
					j=(yy-35-25)/13;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=16+j*8+i;
	//				colset_exchange=tmp_col_sel;
					colset_exchange=-1;
					put_select_color(-1,-1);
					put_load_color(tmp_col_sel);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (10<=xx && xx<10+40 && 3+25+65+10<=yy && yy<3+25+65+10+17) {
					color_load_open_flag=TRUE;
				}
				if (120-40<=xx && xx<120-24 && 3<=yy && yy<15) {
					BYTE	*p=colbuttonBuf;
					for (j=0;j<14;j++)
						for (i=0;i<16;i++) {
							*p++=init_color[pcolor][0];
							*p++=init_color[pcolor][1];
							*p++=init_color[pcolor][2];
						}
					col_counter=5;
					for (j=0;j<winHeight;j++)
						for (i=0;i<winWidth;i++) 
							set_pixel_24(base24Buf,i,j,get_pixel(baseBuf,i,j));
					color_load_mode=COLOR_LOAD_END;
				}
				if (120-20<=xx && xx<120-4 && 3<=yy && yy<15) {
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							init_color[j][i]=tmp_color[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode[j][i]=color_mode_tmp[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode[j][i]=conv_mode_tmp[j][i];
					for (j=0;j<16;j++)
						cconv[j]=cconv_tmp[j];
					for (j=0;j<16;j++)
						area_color[j]=tmp_area_color[j];
					color_fit(-1);
					color_load_mode=COLOR_LOAD_END;
				}
			} else {
				tmp_col_sel=get_pixel(baseBuf,x,y);
				put_select_color(tmp_col_sel,-1);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
		} else {
			if (colset_x<=xx && xx<=190+colset_x && colset_y<=yy && yy<=190+colset_y) {
				xx-=colset_x;
				yy-=colset_y;
				colset_state=COLSET_NONE;
				color_load_open_flag=FALSE;
				colset_exchange=-1;
				tmp_col_sel=-1;

				if (xx<190-40 && yy<17) {
					colload_state=COLLOAD_MOVE;
					colset_move_x=xx;
					colset_move_y=yy;
				}

				if (25<=yy && yy<25+22+17) {
					i=(xx-10)/22;
					j=(yy-25)/22;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=j*8+i;
					colset_exchange=tmp_col_sel;
					put_select_color(tmp_col_sel,-1);
					put_load_color(-1);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (65+25<=yy && yy<65+25+22+17) {
					i=(xx-10)/22;
					j=(yy-65-25)/22;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=16+j*8+i;
	//				colset_exchange=tmp_col_sel;
					colset_exchange=-1;
					put_select_color(-1,-1);
					put_load_color(tmp_col_sel);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (10<=xx && xx<10+40 && 3+65+65+10<=yy && yy<3+65+65+10+17) {
					color_load_open_flag=TRUE;
				}
				if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
					BYTE	*p=colbuttonBuf;
					for (j=0;j<14;j++)
						for (i=0;i<16;i++) {
							*p++=init_color[pcolor][0];
							*p++=init_color[pcolor][1];
							*p++=init_color[pcolor][2];
						}
					col_counter=5;
					for (j=0;j<winHeight;j++)
						for (i=0;i<winWidth;i++) 
							set_pixel_24(base24Buf,i,j,get_pixel(baseBuf,i,j));
					color_load_mode=COLOR_LOAD_END;
				}
				if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							init_color[j][i]=tmp_color[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode[j][i]=color_mode_tmp[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode[j][i]=conv_mode_tmp[j][i];
					for (j=0;j<16;j++)
						cconv[j]=cconv_tmp[j];
					for (j=0;j<16;j++)
						area_color[j]=tmp_area_color[j];
					color_fit(-1);
					color_load_mode=COLOR_LOAD_END;
				}
			} else {
				tmp_col_sel=get_pixel(baseBuf,x,y);
				put_select_color(tmp_col_sel,-1);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
		}
		return;
	}

	if (color_set_mode==COLOR_SET_START) {
		int		i,j,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;

		if (pspc_flag==TRUE) {
			if (colset_x<=xx && xx<=120+colset_x && colset_y<=yy && yy<=110+colset_y) {
				xx-=colset_x;
				yy-=colset_y;
				colset_state=COLSET_NONE;
				colset_exchange=-1;

				if (xx<120-40 && yy<17) {
					colset_state=COLSET_MOVE;
					colset_move_x=xx;
					colset_move_y=yy;
				}
				if (25<=yy && yy<55) {
					i=(xx-10)/13;
					j=(yy-25)/13;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=j*8+i;
					colset_exchange=tmp_col_sel;
					put_select_color(tmp_col_sel,-1);
					set_color_value_red(tmp_col_sel,init_color[tmp_col_sel][2]);
					set_color_value_green(tmp_col_sel,init_color[tmp_col_sel][1]);
					set_color_value_blue(tmp_col_sel,init_color[tmp_col_sel][0]);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (55<=yy) {
					BOOL flag=FALSE;
					j=(yy-55)/17;
					if (j<0) j=0;
					if (j>2) j=2;
					if (40-3<=xx && xx<=40+64+3) {
						i=(xx-45)*4;
						flag=TRUE;
					}
					if (xx<45-3)
						i=init_color[tmp_col_sel][2-j]-1;
					if (40+64+3<xx)
						i=init_color[tmp_col_sel][2-j]+1;
					if (i<0) i=0;
					if (i>255) i=255;
					if (j==0) {
						if (flag) colset_state=COLSET_RED;
						set_color_value_red(tmp_col_sel,i);
					}
					if (j==1) {
						if (flag) colset_state=COLSET_GREEN;
						set_color_value_green(tmp_col_sel,i);
					}
					if (j==2) {
						if (flag) colset_state=COLSET_BLUE;
						set_color_value_blue(tmp_col_sel,i);
					}
					color_fit(tmp_col_sel);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (120-40<=xx && xx<120-24 && 3<=yy && yy<15) {
					BYTE	*p=colbuttonBuf;
					for (j=0;j<14;j++)
						for (i=0;i<16;i++) {
							*p++=init_color[pcolor][0];
							*p++=init_color[pcolor][1];
							*p++=init_color[pcolor][2];
						}
					col_counter=5;
					for (j=0;j<winHeight;j++)
						for (i=0;i<winWidth;i++) 
							set_pixel_24(base24Buf,i,j,get_pixel(baseBuf,i,j));
					color_set_mode=COLOR_SET_END;
				}
				if (120-20<=xx && xx<120-4 && 3<=yy && yy<15) {
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							init_color[j][i]=tmp_color[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode[j][i]=color_mode_tmp[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode[j][i]=conv_mode_tmp[j][i];
					for (j=0;j<16;j++)
						cconv[j]=cconv_tmp[j];
					for (j=0;j<16;j++)
						area_color[j]=tmp_area_color[j];
					color_fit(-1);
					color_set_mode=COLOR_SET_END;
				}
			} else {
				tmp_col_sel=get_pixel(baseBuf,x,y);
				put_select_color(tmp_col_sel,-1);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
		} else {
			if (colset_x<=xx && xx<=190+colset_x && colset_y<=yy && yy<=135+colset_y) {
				xx-=colset_x;
				yy-=colset_y;
				colset_state=COLSET_NONE;
				colset_exchange=-1;

				if (xx<190-40 && yy<17) {
					colset_state=COLSET_MOVE;
					colset_move_x=xx;
					colset_move_y=yy;
				}
				if (25<=yy && yy<25+22+17) {
					i=(xx-10)/22;
					j=(yy-25)/22;
					if (i<0) i=0;
					if (i>=8) i=7;
					if (j<0) j=0;
					if (j>=2) j=1;
					tmp_col_sel=j*8+i;
					colset_exchange=tmp_col_sel;
					put_select_color(tmp_col_sel,-1);
					set_color_value_red(tmp_col_sel,init_color[tmp_col_sel][2]);
					set_color_value_green(tmp_col_sel,init_color[tmp_col_sel][1]);
					set_color_value_blue(tmp_col_sel,init_color[tmp_col_sel][0]);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (70<=yy) {
					BOOL flag=FALSE;
					j=(yy-75)/20;
					if (j<0) j=0;
					if (j>2) j=2;
					if (45-3<=xx && xx<=45+128+3) {
						i=(xx-45)*2;
						flag=TRUE;
					}
					if (xx<45-3)
						i=init_color[tmp_col_sel][2-j]-1;
					if (45+128+3<xx)
						i=init_color[tmp_col_sel][2-j]+1;
					if (i<0) i=0;
					if (i>255) i=255;
					if (j==0) {
						if (flag) colset_state=COLSET_RED;
						set_color_value_red(tmp_col_sel,i);
					}
					if (j==1) {
						if (flag) colset_state=COLSET_GREEN;
						set_color_value_green(tmp_col_sel,i);
					}
					if (j==2) {
						if (flag) colset_state=COLSET_BLUE;
						set_color_value_blue(tmp_col_sel,i);
					}
					color_fit(tmp_col_sel);
					InvalidateRect(hWnd,NULL,FALSE);	
				}
				if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
					BYTE	*p=colbuttonBuf;
					for (j=0;j<14;j++)
						for (i=0;i<16;i++) {
							*p++=init_color[pcolor][0];
							*p++=init_color[pcolor][1];
							*p++=init_color[pcolor][2];
						}
					col_counter=5;
					for (j=0;j<winHeight;j++)
						for (i=0;i<winWidth;i++) 
							set_pixel_24(base24Buf,i,j,get_pixel(baseBuf,i,j));
					color_set_mode=COLOR_SET_END;
				}
				if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
					for (j=0;j<16;j++)
						for (i=0;i<3;i++)
							init_color[j][i]=tmp_color[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							color_mode[j][i]=color_mode_tmp[j][i];
					for (j=0;j<16;j++)
						for (i=0;i<16;i++)
							conv_mode[j][i]=conv_mode_tmp[j][i];
					for (j=0;j<16;j++)
						cconv[j]=cconv_tmp[j];
					for (j=0;j<16;j++)
						area_color[j]=tmp_area_color[j];
					color_fit(-1);
					color_set_mode=COLOR_SET_END;
				}
			} else {
				tmp_col_sel=get_pixel(baseBuf,x,y);
				put_select_color(tmp_col_sel,-1);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
		}
		return;
	}
	if (color_bg_mode==COLOR_BG_START) {
		int		i,j,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;

		colbg_state=COLBG_NONE;
		if (colset_x<=xx && xx<190+colset_x && colset_y<=yy && yy<90+colset_y) {
			xx-=colset_x;
			yy-=colset_y;

			if (25<=yy && yy<25+15) {
				tmp_bgc_color=16;
				init_color_bg_dialog(tmp_bgc_color);
			}
			if (25+22<=yy && yy<25+22*2+15) {
				i=(xx-10)/22;
				j=(yy-22-25)/22;
				if (i<0) i=0;
				if (i>=8) i=7;
				if (j<0) j=0;
				if (j>=2) j=1;
				tmp_bgc_color=j*8+i;
				init_color_bg_dialog(tmp_bgc_color);
			}

			if (xx<190-40 && yy<17) {
				colbg_state=COLBG_MOVE;
				colset_move_x=xx;
				colset_move_y=yy;
			}
			if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
				bgc=tmp_bgc_color;
				color_bg_mode=COLOR_BG_END;
			}
			if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
				color_bg_mode=COLOR_BG_END;
			}
			InvalidateRect(hWnd,NULL,FALSE);	
		} else {
			tmp_bgc_color=get_pixel(baseBuf,x,y);
			init_color_bg_dialog(tmp_bgc_color);
			InvalidateRect(hWnd,NULL,FALSE);	
		}
		return;
	}
	if (color_area_mode==COLOR_AREA_START) {
		int		i,j,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;

		colarea_state=COLAREA_NONE;
		if (colset_x<=xx && xx<190+colset_x && colset_y<=yy && yy<70+colset_y) {
			xx-=colset_x;
			yy-=colset_y;

			if (25<=yy && yy<25+22+15) {
				i=(xx-10)/22;
				j=(yy-25)/22;
				if (i<0) i=0;
				if (i>=8) i=7;
				if (j<0) j=0;
				if (j>=2) j=1;
				if (area_color[j*8+i]==TRUE)	area_color[j*8+i]=FALSE;
				else							area_color[j*8+i]=TRUE;
				init_color_area_dialog();
			}

			if (xx<190-40 && yy<17) {
				colarea_state=COLAREA_MOVE;
				colset_move_x=xx;
				colset_move_y=yy;
			}
			if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
				color_area_mode=COLOR_AREA_END;
			}
			if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
				for (i=0;i<16;i++)
					area_color[i]=tmp_area_color[i];
				color_area_mode=COLOR_AREA_END;
			}
			InvalidateRect(hWnd,NULL,FALSE);	
		} else {
			BYTE col;
			col=get_pixel(baseBuf,x,y);
			if (area_color[col]==TRUE)	area_color[col]=FALSE;
			else						area_color[col]=TRUE;
			init_color_area_dialog();
			InvalidateRect(hWnd,NULL,FALSE);	
		}
		return;
	}
	if (color_conv_mode==COLOR_CONV_START) {
		int		i,j,k,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;

		colconv_state=COLCONV_NONE;
		if (colset_x<=xx && xx<190+colset_x && colset_y<=yy && yy<190+colset_y) {
			xx-=colset_x;
			yy-=colset_y;

			if (22<=xx && xx<16*10+22 && 31<=yy && yy<16*10+31) {
				i=(xx-22)/10;
				j=(yy-31)/10;
				for (k=0;k<16;k++)
					conv_mode[j][k]=FALSE; 
				conv_mode[j][i]=TRUE;
				cconv[j]=i;
				init_color_conv_dialog(color_row,color_column);
			}
			if (9<=xx && xx<22 && 18<=yy && yy<31) {
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						conv_mode[j][i]=FALSE;
				for (j=0;j<16;j++) {
					conv_mode[j][j]=TRUE;
					cconv[j]=j;
				}
				init_color_conv_dialog(color_row,color_column);
			}
			if (22<=xx && 18<=yy && yy<31) {
				color_column=(xx-22)/10;
				if (color_column<0) color_column=0;
				if (color_column>15) color_column=15;
				init_color_conv_dialog(color_row,color_column);
			}
			if (9<=xx && xx<22 && 31<=yy) {
				color_row=(yy-31)/10;
				if (color_row<0) color_row=0;
				if (color_row>15) color_row=15;
				init_color_conv_dialog(color_row,color_column);
			}

			if (xx<190-40 && yy<17) {
				colconv_state=COLCONV_MOVE;
				colset_move_x=xx;
				colset_move_y=yy;
			}
			if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
				color_conv_mode=COLOR_CONV_END;
			}
			if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						conv_mode[j][i]=conv_mode_tmp[j][i];
				for (j=0;j<16;j++)
					cconv[j]=cconv_tmp[j];
				color_conv_mode=COLOR_CONV_END;
			}
			InvalidateRect(hWnd,NULL,FALSE);	
		} else {
			color_column=get_pixel(baseBuf,x,y);
			init_color_conv_dialog(color_row,color_column);
			InvalidateRect(hWnd,NULL,FALSE);	
		}
		return;
	}
	if (color_mask_mode==COLOR_MODE_START) {
		int		i,j,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;

		colmode_state=COLMODE_NONE;
		if (colset_x<=xx && xx<190+colset_x && colset_y<=yy && yy<190+colset_y) {
			xx-=colset_x;
			yy-=colset_y;

			if (22<=xx && xx<16*10+22 && 31<=yy && yy<16*10+31) {
				i=(xx-22)/10;
				j=(yy-31)/10;
				if (color_mode[j][i]==TRUE)
					color_mode[j][i]=FALSE;
				else
					color_mode[j][i]=TRUE;
				init_color_mode_dialog();
			}
			if (9<=xx && xx<22 && 18<=yy && yy<31) {
				BOOL c=(color_mode[0][0]) ? FALSE : TRUE;
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						color_mode[j][i]=c;
				init_color_mode_dialog();
			}
			if (22<=xx && 18<=yy && yy<31) {
				BOOL c=(color_mode[0][color_column]) ? FALSE : TRUE;
				color_column=(xx-22)/10;
				for (j=0;j<16;j++)
					color_mode[j][color_column]=c;
				init_color_mode_dialog();
			}
			if (9<=xx && xx<22 && 31<=yy) {
				BOOL c=(color_mode[color_row][0]) ? FALSE : TRUE;
				color_row=(yy-31)/10;
				for (j=0;j<16;j++)
					color_mode[color_row][j]=c;
				init_color_mode_dialog();
			}

			if (xx<190-40 && yy<17) {
				colmode_state=COLMODE_MOVE;
				colset_move_x=xx;
				colset_move_y=yy;
			}
			if (190-40<=xx && xx<190-24 && 3<=yy && yy<15) {
				color_mask_mode=COLOR_MODE_END;
			}
			if (190-20<=xx && xx<190-4 && 3<=yy && yy<15) {
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						color_mode[j][i]=color_mode_tmp[j][i];
				color_mask_mode=COLOR_MODE_END;
			}
			InvalidateRect(hWnd,NULL,FALSE);	
		} else {
			color_column=get_pixel(baseBuf,x,y);
			init_color_mode_dialog();
			InvalidateRect(hWnd,NULL,FALSE);	
		}
		return;
	}
	
	if (region_flag==HAND) {
		x = (LOWORD(lParam));
		y = (HIWORD(lParam));
		if (GetTickCount()-ticks<150) {
		} else {
			hand_x=x;
			hand_y=y;
			hand_lx=loope_x;
			hand_ly=loope_y;
		}
		return;
	}
	if (region_flag==PAINT) {
		paint(hWnd,x,y);
		return;
	}
	if (region_flag==SPUIT) {
		BYTE	*p=colbuttonBuf;
		int		i,j;
		pcolor=get_pixel(baseBuf,x,y);
		for (j=0;j<14;j++)
			for (i=0;i<16;i++) {
				*p++=init_color[pcolor][0];
				*p++=init_color[pcolor][1];
				*p++=init_color[pcolor][2];
			}
		col_counter=5;
		return;
	}
	if (region_flag==FONT) {
		font_x=x;
		font_y=y;
		input_text(hWnd);
		return;
	}
	
	if (region_flag==DRAG) {
		drag_tap_flag=-1;
		if (GetTickCount()-ticks<500) {
			select_dx=x-select_x;
			select_dy=y-select_y;
		} else {
			int sx,sy,ex,ey;
			sx=(region_sx<region_ex) ? region_sx+select_dx : region_ex+select_dx;
			sy=(region_sy<region_ey) ? region_sy+select_dy : region_ey+select_dy;
			ex=(region_sx>region_ex) ? region_sx+select_dx : region_ex+select_dx;
			ey=(region_sy>region_ey) ? region_sy+select_dy : region_ey+select_dy;
			if (rotate_flag%2==1) {
				int ex2,ey2;
				ex2=sx+(ey-sy);
				ey2=sy+(ex-sx);
				ex=ex2;
				ey=ey2;
			}
			if (x<sx || ex<x || y<sy || ey<y) {
				paste(hWnd);
				modified_flag=1;
				select_dx=0;
				select_dy=0;
				region_flag=CANSEL;
				toolpic=TOOLPIC_PEN;
				make_command_bar(hWnd);
			} else {
				memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
				select_x=x-select_dx;
				select_y=y-select_dy;
				select_dx=x-select_x;
				select_dy=y-select_y;
			}
		}
		return;
	}
	if (region_flag==SELECT_END) {
		if (GetTickCount()-ticks<300) {
			region_flag=SELECT;
			return;
		} else {
			region_flag=SELECT_START;
		}
		InvalidateRect(hWnd,NULL,FALSE); 
	}
	if (region_flag==SELECT_FREE_END) {
		if (GetTickCount()-ticks<300) {
			region_flag=SELECT_FREE;
			return;
		} else {
			region_flag=SELECT_FREE_START;
		}
	}

	if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
		line_tm=-1;
		if (GetTickCount()-ticks<250) return;
		if (region_sx<0) {
			region_sx=x;
			region_sy=y;
		}
		region_ex=x;
		region_ey=y;
		undo_store(hWnd);
		return;
	}

	if (region_flag==SELECT_START) {
		if (start_select_rect_flag==AREA_RECT) {
			select_region(hWnd);
			region_sx=x;
			region_sy=y;
		}
		if (start_select_rect_flag==AREA_ALL) {
			select_all(hWnd);
		}
	}
	if (region_flag==SELECT) {
		if (start_select_rect_flag==AREA_MAGICWAND) {
			region_sx=0;
			region_sy=0;
			region_ex=winWidth-1;
			region_ey=winHeight-1;
			magicwand(x,y);
			InvalidateRect(hWnd,NULL,FALSE);
			return;
		} else {
			memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
			region_ex=x;
			region_ey=y;
			return;
		}
	}
	if (region_flag==SELECT_FREE_START) {
		if (start_select_rect_flag==AREA_FREE) select_region(hWnd);
		select_free(hWnd);
		region_flag=SELECT_FREE;
		pts[0].x=-1;
		pts[0].y=-1;
		pts[1].x=x;
		pts[1].y=y;
		pts[2].x=x;
		pts[2].y=y;
		select_free_x=x;
		select_free_y=y;
		region_sx=x;
		region_sy=y;
		region_ex=x;
		region_ey=y;
	}
	if (region_flag==SELECT_FREE) {
		if (region_sx>x) region_sx=x;
		if (region_sy>y) region_sy=y;
		if (region_ex<x) region_ex=x;
		if (region_ey<y) region_ey=y;
		return;
	}
	if (region_flag==ERASE) {
		erase_put_flag=TRUE;
		erase_x=x-style[1][perase]/2;
		erase_y=y-style[1][perase]/2;
	}
	undo_store(hWnd);

//	pts[0].x=-1;
//	pts[0].y=-1;
	pts[1].x=x;
	pts[1].y=y;

	put_pen(x,y,baseBuf,region_flag==PEN ? pcolor : 3,region_flag==PEN ? pstyle : perase,TRUE,FALSE);
	modified_flag=1;
	{
		RECT rc;
		int	r=style[region_flag==PEN ? 0:1][region_flag==PEN ? pstyle : perase];
		rc.left=x-r;
		rc.top=y-r; 
		rc.right=x+r;
		rc.bottom=y+r;
		if (loope_mag!=1) {
			RECT	rc2;
			rc2.left=rc.left-loope_x;
			rc2.top=rc.top-loope_y+cbh;

			rc2.right=rc.right-loope_x;
			rc2.bottom=rc.bottom-loope_y+cbh;
			InvalidateRect(hWnd,&rc2,FALSE);

			rc.left=(rc.left-loope_x)*loope_mag;
			rc.top=(rc.top-loope_y)*loope_mag+cbh;
			rc.right=(rc.right-loope_x)*loope_mag;
			rc.bottom=(rc.bottom-loope_y)*loope_mag+cbh;
		} else {
			rc.left-=loope_x;
			rc.right-=loope_x;
			rc.top+=cbh-loope_y;
			rc.bottom+=cbh-loope_y;
		}
		InvalidateRect(hWnd,&rc,FALSE);	
	}
}

void mouse_move(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RECT        rc;
	int			x,y,p,i;
	BYTE or_mask[4]={0xc0,0x30,0x0c,0x03};
	BYTE and_mask[4]={0x3f,0xcf,0xf3,0xfc};
	BYTE shift[4]={6,4,2,0};

	line_adjust++;
	if (line_adjust<=line_adjust_max) return;
	else line_adjust=0;

	if (mouse_flag==UP) return;
	mouse_flag=MOVE;

	GetWindowRect(hWnd, &rc);
	x = (LOWORD(lParam))/loope_mag+loope_x;
	y = (HIWORD(lParam)-cbh)/loope_mag+loope_y;

	if (color_bg_mode==COLOR_BG_END)	return;
	if (color_bg_mode==COLOR_BG_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colbg_state==COLBG_MOVE) {
			colset_x=xx-colset_move_x;
			colset_y=yy-colset_move_y;
			if (colset_x<0) colset_x=0;
			if (winWidth<=colset_x+190) colset_x=winWidth-190;
			if (colset_y<0) colset_y=0;
			if (winHeight<=colset_y+90) colset_y=winHeight-90;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return;
	}
	if (color_mask_mode==COLOR_MODE_END)	return;
	if (color_mask_mode==COLOR_MODE_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colmode_state==COLMODE_MOVE) {
			colset_x=xx-colset_move_x;
			colset_y=yy-colset_move_y;
			if (colset_x<0) colset_x=0;
			if (winWidth<=colset_x+190) colset_x=winWidth-190;
			if (colset_y<0) colset_y=0;
			if (winHeight<=colset_y+190) colset_y=winHeight-190;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return;
	}
	if (color_conv_mode==COLOR_CONV_END)	return;
	if (color_conv_mode==COLOR_CONV_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colconv_state==COLCONV_MOVE) {
			colset_x=xx-colset_move_x;
			colset_y=yy-colset_move_y;
			if (colset_x<0) colset_x=0;
			if (winWidth<=colset_x+190) colset_x=winWidth-190;
			if (colset_y<0) colset_y=0;
			if (winHeight<=colset_y+190) colset_y=winHeight-190;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return;
	}
	if (color_area_mode==COLOR_AREA_END)	return;
	if (color_area_mode==COLOR_AREA_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colarea_state==COLAREA_MOVE) {
			colset_x=xx-colset_move_x;
			colset_y=yy-colset_move_y;
			if (colset_x<0) colset_x=0;
			if (winWidth<=colset_x+190) colset_x=winWidth-190;
			if (colset_y<0) colset_y=0;
			if (winHeight<=colset_y+70) colset_y=winHeight-70;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return;
	}
	if (color_bg_mode==COLOR_BG_START || color_bg_mode==COLOR_BG_END)	return;
	if (color_area_mode==COLOR_AREA_START || color_area_mode==COLOR_AREA_END)	return;

	if (color_set_mode ==COLOR_SET_END)	return;
	if (color_set_mode==COLOR_SET_START) {
		int		i,xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (pspc_flag==TRUE) {
			if (colset_state==COLSET_MOVE) {
				colset_x=xx-colset_move_x;
				colset_y=yy-colset_move_y;
				if (colset_x<0) colset_x=0;
				if (winWidth<=colset_x+120) colset_x=winWidth-120;
				if (colset_y<0) colset_y=0;
				if (winHeight<=colset_y+110) colset_y=winHeight-110;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			xx-=colset_x;
			yy-=colset_y;
			if (colset_state==COLSET_RED || colset_state==COLSET_GREEN || colset_state==COLSET_BLUE) {
				i=(xx-45)*4;	
			if (i<0) i=0;
				if (i>255) i=255;
				if (colset_state==COLSET_RED)	set_color_value_red(tmp_col_sel,i);
				if (colset_state==COLSET_GREEN) set_color_value_green(tmp_col_sel,i);
				if (colset_state==COLSET_BLUE)	set_color_value_blue(tmp_col_sel,i);

				color_fit(tmp_col_sel);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
			if (colset_exchange!=-1) {
				int i,j;
				i=(xx-10)/13;
				j=(yy-25)/13;
				if (i<0) i=0;
				if (i>=8) i=7;
				if (j<0) j=0;
				if (j>=2) j=1;
				colset_exchange=j*8+i;
				put_select_color(tmp_col_sel,colset_exchange); 
				InvalidateRect(hWnd,NULL,FALSE);
			}
		} else {
			if (colset_state==COLSET_MOVE) {
				colset_x=xx-colset_move_x;
				colset_y=yy-colset_move_y;
				if (colset_x<0) colset_x=0;
				if (winWidth<=colset_x+190) colset_x=winWidth-190;
				if (colset_y<0) colset_y=0;
				if (winHeight<=colset_y+135) colset_y=winHeight-135;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			xx-=colset_x;
			yy-=colset_y;
			if (colset_state==COLSET_RED || colset_state==COLSET_GREEN || colset_state==COLSET_BLUE) {
				i=(xx-50)*2;	
			if (i<0) i=0;
				if (i>255) i=255;
				if (colset_state==COLSET_RED)	set_color_value_red(tmp_col_sel,i);
				if (colset_state==COLSET_GREEN) set_color_value_green(tmp_col_sel,i);
				if (colset_state==COLSET_BLUE)	set_color_value_blue(tmp_col_sel,i);

				color_fit(tmp_col_sel);
				InvalidateRect(hWnd,NULL,FALSE);	
			}
			if (colset_exchange!=-1) {
				int i,j;
				i=(xx-10)/22;
				j=(yy-25)/22;
				if (i<0) i=0;
				if (i>=8) i=7;
				if (j<0) j=0;
				if (j>=2) j=1;
				colset_exchange=j*8+i;
				put_select_color(tmp_col_sel,colset_exchange); 
				InvalidateRect(hWnd,NULL,FALSE);
			}
		}
		return;
	}

	if (color_load_mode ==COLOR_LOAD_END)	return;
	if (color_load_mode==COLOR_LOAD_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colload_state==COLLOAD_MOVE) {
			colset_x=xx-colset_move_x;
			colset_y=yy-colset_move_y;
			if (colset_x<0) colset_x=0;
			if (winWidth<=colset_x+(pspc_flag ? 120:190)) colset_x=winWidth-(pspc_flag ? 120:190);
			if (colset_y<0) colset_y=0;
			if (winHeight<=colset_y+(pspc_flag ? 125:190)) colset_y=winHeight-(pspc_flag? 125:190);
			InvalidateRect(hWnd,NULL,FALSE);
		}
		xx-=colset_x;
		yy-=colset_y;

		if (!(10<=xx && xx<10+40 && 3+(pspc_flag ? 25:65)+65+10<=yy && yy<3+(pspc_flag ? 25:65)+65+10+17)) {
			color_load_open_flag=FALSE;
		}
		colset_exchange=-1;
		if (tmp_col_sel!=-1) {
			if (25<=yy && yy<25+(pspc_flag ? 13+8:22+17)) {
				int i,j;
				i=(xx-10)/(pspc_flag ? 13:22);
				j=(yy-25)/(pspc_flag ? 13:22);
				if (i<0) i=0;
				if (i>=8) i=7;
				if (j<0) j=0;
				if (j>=2) j=1;
				colset_exchange=j*8+i;
			}
			put_select_color(tmp_col_sel,colset_exchange); 
			put_load_color(tmp_col_sel); 
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return;
	}

	if (region_flag==SELECT) {
		if (start_select_rect_flag==AREA_MAGICWAND) 
			return;
	}
	if (region_flag==SPUIT) {
		BYTE	*p=colbuttonBuf;
		int		i,j;
		pcolor=get_pixel(baseBuf,x,y);
		for (j=0;j<14;j++)
			for (i=0;i<16;i++) {
				*p++=init_color[pcolor][0];
				*p++=init_color[pcolor][1];
				*p++=init_color[pcolor][2];
			}
		col_counter=5;
		return;
	}
	if (region_flag==HAND) {
		x = (LOWORD(lParam));
		y = (HIWORD(lParam));
		loope_x=hand_lx-(x-hand_x)/loope_mag;
		loope_y=hand_ly-(y-hand_y)/loope_mag;
		if (loope_x<-winWidth/loope_mag/2)				loope_x=-winWidth/loope_mag/2;
		if (loope_y<-winHeight/loope_mag/2)				loope_y=-winHeight/loope_mag/2;
		if (winWidth-winWidth/loope_mag/2<loope_x)		loope_x=winWidth-winWidth/loope_mag/2;
		if (winHeight-winHeight/loope_mag/2<loope_y)	loope_y=winHeight-winHeight/loope_mag/2;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (region_flag==DRAG) {
		select_dx=x-select_x;
		select_dy=y-select_y;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (region_flag==CANSEL) {
		return;
	}
	if (region_flag==PAINT) {
		return;
	}
	if (region_flag==FONT) {
		return;
	}
	if (region_flag==SELECT_END) {
		memcpy(tmpBuf,baseBuf,winWidth*winHeight/2);
		region_ex=x;
		region_ey=y;
		region_flag=SELECT;
	}
	if (region_flag==SELECT) {
		region_ex=x;
		region_ey=y;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	
	if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
		region_ex=x;
		region_ey=y;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	
	if (region_flag==SELECT_FREE_END) {
		region_flag=SELECT_FREE;
	}
	if (region_flag==SELECT_FREE) {
/*
		if (x<0) x=0;
		if (y<0) y=0;
		if (winWidth-1<=x)	x=winWidth-1;
		if (winHeight-1<=y)	y=winHeight-1;
*/
		draw_line(pts[2].x,pts[2].y,x,y,freeareaBuf,1,0,FALSE,TRUE);
		pts[0].x=pts[1].x;
		pts[0].y=pts[1].y;
		pts[1].x=pts[2].x;
		pts[1].y=pts[2].y;
		pts[2].x=x; 
		pts[2].y=y;

		if (x<region_sx) region_sx=x;
		if (y<region_sy) region_sy=y;
		if (x>region_ex) region_ex=x;
		if (y>region_ey) region_ey=y;
		rc.left  =region_sx;
		rc.top   =region_sy;
		rc.right =region_ex;
		rc.bottom=region_ey;
		rc.top+=cbh;
		rc.bottom+=cbh;
		if (loope_mag==1)
			InvalidateRect(hWnd,&rc,FALSE);
		else
			InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	if (region_flag==ERASE) {
		erase_put_flag=TRUE;
		erase_x=x-style[1][perase]/2;
		erase_y=y-style[1][perase]/2;
	}

	pts[2].x=x; 
	pts[2].y=y;
	rc.top=999;
	rc.left=999;
	rc.bottom=-999;
	rc.right=-999;
	for (i=1;i<3;i++) {
		if (pts[i].x<0) continue;
		if (pts[i].x<rc.left) rc.left=pts[i].x;
		if (pts[i].x>rc.right) rc.right=pts[i].x;
		if (pts[i].y<rc.top) rc.top=pts[i].y;
		if (pts[i].y>rc.bottom) rc.bottom=pts[i].y;
	} 
	p=style[region_flag==ERASE ? 1 : 0][region_flag==ERASE ? perase : pstyle];
	rc.top-=p+1;
	rc.left-=p+1;
	rc.bottom+=p+1;
	rc.right+=p;	

	if (rc.top<0) rc.top=0;
	if (rc.bottom>winHeight) rc.bottom=winHeight;
	if (rc.left<0) rc.left=0;
	if (rc.right>winWidth-1) rc.right=winWidth;

	draw_line(pts[1].x,pts[1].y,pts[2].x,pts[2].y,baseBuf,
		pcolor,region_flag==PEN ? pstyle : perase,TRUE,FALSE);
		
	if (loope_mag!=1) {
		RECT	rc2;
		rc2.left=rc.left-loope_x;
		rc2.top=rc.top-loope_y+cbh;
		rc2.right=rc.right-loope_x;
		rc2.bottom=rc.bottom-loope_y+cbh;
		InvalidateRect(hWnd,&rc2,FALSE);

		rc.left=(rc.left-loope_x)*loope_mag;
		rc.top=(rc.top-loope_y)*loope_mag+cbh;
		rc.right=(rc.right-loope_x)*loope_mag;
		rc.bottom=(rc.bottom-loope_y)*loope_mag+cbh;
	} else {
		rc.left-=loope_x;
		rc.right-=loope_x;
		rc.top+=cbh-loope_y;
		rc.bottom+=cbh-loope_y;
	}
	InvalidateRect(hWnd,&rc,FALSE);
//	pts[0].x=pts[1].x;
//	pts[0].y=pts[1].y;
	pts[1].x=pts[2].x;
	pts[1].y=pts[2].y;
}

void mouse_up(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	BYTE or_mask[4]={0xc0,0x30,0x0c,0x03};
	BYTE and_mask[4]={0x3f,0xcf,0xf3,0xfc};
	BYTE shift[4]={6,4,2,0};
	int			x,y;

	mouse_flag=UP;
	ReleaseCapture();

	x = (LOWORD(lParam))/loope_mag+loope_x;
	y = (HIWORD(lParam)-cbh)/loope_mag+loope_y;

	
	if (color_area_mode==COLOR_AREA_START) return;
	if (colarea_state==COLAREA_MOVE) {
		colarea_state=COLAREA_MOVE2;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (color_area_mode==COLOR_AREA_END) {
		color_area_mode=COLOR_AREA_NONE;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	if (color_bg_mode==COLOR_BG_START) return;
	if (colbg_state==COLBG_MOVE) {
		colbg_state=COLBG_MOVE2;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (color_bg_mode==COLOR_BG_END) {
		color_bg_mode=COLOR_BG_NONE;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	if (color_mask_mode==COLOR_MODE_START) return;
	if (colmode_state==COLMODE_MOVE) {
		colmode_state=COLMODE_MOVE2;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (color_mask_mode==COLOR_MODE_END) {
		color_mask_mode=COLOR_MODE_NONE;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	if (color_conv_mode==COLOR_CONV_START) return;
	if (colconv_state==COLCONV_MOVE) {
		colconv_state=COLCONV_MOVE2;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}
	if (color_conv_mode==COLOR_CONV_END) {
		color_conv_mode=COLOR_CONV_NONE;
		InvalidateRect(hWnd,NULL,FALSE);
		return;
	}

	if (color_set_mode==COLOR_SET_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colset_x<=xx && xx<=(pspc_flag ? 120 : 190)+colset_x && colset_y<=yy && yy<=(pspc_flag ? 110:135)+colset_y) {
			if (tmp_col_sel!=colset_exchange && colset_exchange!=-1) {
				int i,j,k;
				BYTE tmp;
				BOOL b;
				tmp=init_color[tmp_col_sel][0];
				init_color[tmp_col_sel][0]=init_color[colset_exchange][0];
				init_color[colset_exchange][0]=tmp;
				tmp=init_color[tmp_col_sel][1];
				init_color[tmp_col_sel][1]=init_color[colset_exchange][1];
				init_color[colset_exchange][1]=tmp;
				tmp=init_color[tmp_col_sel][2];
				init_color[tmp_col_sel][2]=init_color[colset_exchange][2];
				init_color[colset_exchange][2]=tmp;

				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(baseBuf,i,j)==tmp_col_sel) {
							set_pixel(baseBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(baseBuf,i,j)==colset_exchange)
								set_pixel(baseBuf,i,j,tmp_col_sel);
						}
					}
				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(undoBuf,i,j)==tmp_col_sel) {
							set_pixel(undoBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(undoBuf,i,j)==colset_exchange)
								set_pixel(undoBuf,i,j,tmp_col_sel);
						}
					}
				
				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(copyBuf,i,j)==tmp_col_sel) {
							set_pixel(copyBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(copyBuf,i,j)==colset_exchange)
								set_pixel(copyBuf,i,j,tmp_col_sel);
						}
					}
			
				b=area_color[tmp_col_sel];
				area_color[tmp_col_sel]=area_color[colset_exchange];
				area_color[colset_exchange]=b;
	
				k=cconv[tmp_col_sel];
				cconv[tmp_col_sel]=cconv[colset_exchange];
				cconv[colset_exchange]=k;
				for (j=0;j<16;j++) {
					if (cconv[j]==tmp_col_sel) {
						cconv[j]=colset_exchange;
					} else {
						if (cconv[j]==colset_exchange)
								cconv[j]=tmp_col_sel;
					}
				}
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						conv_mode[j][i]=(cconv[j]==i) ? TRUE : FALSE;
		
				for (i=0;i<16;i++) {
					b=color_mode[tmp_col_sel][i];
					color_mode[tmp_col_sel][i]=color_mode[colset_exchange][i];
					color_mode[colset_exchange][i]=b;
				}
				for (j=0;j<16;j++) {
					b=color_mode[j][tmp_col_sel];
					color_mode[j][tmp_col_sel]=color_mode[j][colset_exchange];
					color_mode[j][colset_exchange]=b;
				}
					
				tmp_col_sel=colset_exchange;
				put_select_color(tmp_col_sel,colset_exchange);
//				color_fit(-1);
				InvalidateRect(hWnd,NULL,FALSE);
			}
		}
		if (colset_state==COLSET_MOVE) {
			colset_state=COLSET_MOVE2;
			InvalidateRect(hWnd,NULL,FALSE);
			return;
		}
		colset_state=COLSET_NONE;
		return;
	}
	if (color_set_mode==COLOR_SET_END) {
		InvalidateRect(hWnd,NULL,FALSE);
		color_set_mode=COLOR_SET_NONE;
		return;
	}
	
	if (color_load_mode==COLOR_LOAD_START) {
		int		xx,yy;
		xx = LOWORD(lParam);
		yy = HIWORD(lParam)-cbh;
		if (colset_x<=xx && xx<=(pspc_flag ? 120:190)+colset_x && colset_y<=yy && yy<=(pspc_flag ? 125:190)+colset_y) {
			if (color_load_open_flag==TRUE) {
				load_palette(hWnd);
				put_load_color(-1);
				InvalidateRect(hWnd,NULL,FALSE);
			}
			color_load_open_flag=FALSE;
			if (0<=tmp_col_sel && tmp_col_sel<16 && tmp_col_sel!=colset_exchange && colset_exchange!=-1) {
				int i,j,k;
				BYTE tmp;
				BOOL b;
				tmp=init_color[tmp_col_sel][0];
				init_color[tmp_col_sel][0]=init_color[colset_exchange][0];
				init_color[colset_exchange][0]=tmp;
				tmp=init_color[tmp_col_sel][1];
				init_color[tmp_col_sel][1]=init_color[colset_exchange][1];
				init_color[colset_exchange][1]=tmp;
				tmp=init_color[tmp_col_sel][2];
				init_color[tmp_col_sel][2]=init_color[colset_exchange][2];
				init_color[colset_exchange][2]=tmp;
	
				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(baseBuf,i,j)==tmp_col_sel) {
							set_pixel(baseBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(baseBuf,i,j)==colset_exchange)
								set_pixel(baseBuf,i,j,tmp_col_sel);
						}
					}
				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(undoBuf,i,j)==tmp_col_sel) {
							set_pixel(undoBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(undoBuf,i,j)==colset_exchange)
								set_pixel(undoBuf,i,j,tmp_col_sel);
						}
					}
				
				for (j=0;j<winHeight;j++)
					for (i=0;i<winWidth;i++) {
						if (get_pixel(copyBuf,i,j)==tmp_col_sel) {
							set_pixel(copyBuf,i,j,colset_exchange);
						} else {
							if (get_pixel(copyBuf,i,j)==colset_exchange)
								set_pixel(copyBuf,i,j,tmp_col_sel);
						}
					}
				
				b=area_color[tmp_col_sel];
				area_color[tmp_col_sel]=area_color[colset_exchange];
				area_color[colset_exchange]=b;	
	
				k=cconv[tmp_col_sel];
				cconv[tmp_col_sel]=cconv[colset_exchange];
				cconv[colset_exchange]=k;
				for (j=0;j<16;j++) {
					if (cconv[j]==tmp_col_sel) {
						cconv[j]=colset_exchange;
					} else {
						if (cconv[j]==colset_exchange)
								cconv[j]=tmp_col_sel;
					}
				}
				for (j=0;j<16;j++)
					for (i=0;i<16;i++)
						conv_mode[j][i]=(cconv[j]==i) ? TRUE : FALSE;
	
				for (i=0;i<16;i++) {
					b=color_mode[tmp_col_sel][i];
					color_mode[tmp_col_sel][i]=color_mode[colset_exchange][i];
					color_mode[colset_exchange][i]=b;
				}
				for (j=0;j<16;j++) {
					b=color_mode[j][tmp_col_sel];
					color_mode[j][tmp_col_sel]=color_mode[j][colset_exchange];
					color_mode[j][colset_exchange]=b;
				}
				
				tmp_col_sel=colset_exchange;
				put_select_color(-1,-1);
				InvalidateRect(hWnd,NULL,FALSE);
			}
			if (16<=tmp_col_sel && tmp_col_sel<16+16 && colset_exchange!=-1) {
				init_color[colset_exchange][0]=init_color[tmp_col_sel+5][0];
				init_color[colset_exchange][1]=init_color[tmp_col_sel+5][1];
				init_color[colset_exchange][2]=init_color[tmp_col_sel+5][2];
				put_select_color(-1,-1);
				put_load_color(-1);
				InvalidateRect(hWnd,NULL,FALSE);
			}
			if (colload_state==COLLOAD_MOVE) {
				colload_state=COLLOAD_MOVE2;
				InvalidateRect(hWnd,NULL,FALSE);
				return;
			}
			colload_state=COLLOAD_NONE;
			return;
		}
	}
	if (color_load_mode==COLOR_LOAD_END) {
		InvalidateRect(hWnd,NULL,FALSE);
		color_load_mode=COLOR_LOAD_NONE;
		return;
	}
	
	if (color_bg_mode==COLOR_BG_START) return;
	if (color_bg_mode==COLOR_BG_END) {
		color_bg_mode=COLOR_BG_NONE;
		return;
	}
	
	if (color_area_mode==COLOR_AREA_START) return;
	if (color_area_mode==COLOR_AREA_END) {
		color_area_mode=COLOR_AREA_NONE;
		return;
	}
	
	if (region_flag==SPUIT) {
		region_flag=CANSEL;
	}
	if (region_flag==CANSEL) {
		region_flag=PEN;
		toolpic=TOOLPIC_PEN;
		make_command_bar(hWnd);
		return;
	}	
	if (region_flag==SELECT) {
		if (start_select_rect_flag==AREA_MAGICWAND) 
			return;
	}
	
	if (region_flag==PAINT) {
		return;
	}
	if (region_flag==FONT) {
		return;
	}

	if (region_flag==HAND) {
		ticks=GetTickCount();
		return;
	}
	if (region_flag==LINE || region_flag==RECTANGLE || region_flag==CIRCLE) {
//		region_flag=NONE;
		line_tm=tm;
		ticks=GetTickCount();
		return;
	}

	if (region_flag==SELECT) {
		select_x=select_dx=(region_sx+region_ex)/2;
		select_y=select_dy=(region_sy+region_ey)/2;
		region_flag=SELECT_END;
		ticks=GetTickCount();
		return;
	}
	if (region_flag==DRAG) {
		ticks=GetTickCount();
/*
		if (drag_tap_flag==0) {
			if (ticks-drag_tm<300) {
				if (-10<drag_x-x && drag_x-x<10 && -10<drag_y-y && drag_y-y<10) {
					drag_tap_flag=1;
					return;
				}
			}
		}
		drag_tap_flag=-1;
*/
		return;
	}

	if (region_flag==SELECT_FREE) {
		ticks=GetTickCount();
		region_flag=SELECT_FREE_END;				
		return;
	}

	if (region_flag==ERASE) {
		erase_put_flag=FALSE;
	}
}

void do_undo(HWND hWnd)
{
	int		i,j;
	memcpy(tmpBuf, baseBuf,winWidth*winHeight/2);
	memcpy(baseBuf,undoBuf,winWidth*winHeight/2);
	memcpy(undoBuf,tmpBuf ,winWidth*winHeight/2);
	for (j=0;j<winHeight;j++)
		for (i=0;i<winWidth;i++)
			set_pixel_24(base24Buf,i,j,get_pixel(baseBuf,i,j));
	InvalidateRect(hWnd,NULL,FALSE);
}

void undo_store(HWND hWnd)
{
	memcpy(undoBuf,baseBuf,winWidth*winHeight/2);
}

/* =====================================================================
	ヘルプボタンが押された
===================================================================== */
void OnHelp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	OnHelpAbout(hWnd); 
}

/* =====================================================================
	ファイル・メニュー - 終了
===================================================================== */
void OnFileExit(HWND hWnd) 
{
	 put_colbutton_flag=FALSE;
	if (DialogBox(ghInst, MAKEINTRESOURCE(IDD_EXIT_DIALOG), hWnd, SaveProc)!=IDCANCEL)
		SendMessage(hWnd, WM_CLOSE, 0, 0L);
	 put_colbutton_flag=TRUE;
}

/* =====================================================================
	ヘルプ・メニュー - バージョン情報の表示
===================================================================== */
void OnHelpAbout(HWND hWnd)
{
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUT_DIALOG),
		hWnd, AboutDlgProc);
}

/* =====================================================================
    バージョン情報の表示ダイアログ
===================================================================== */
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMessage,
						   WPARAM wParam, LPARAM lParam)
{
	switch (uMessage) {
	  case WM_INITDIALOG:			// ダイアログ・ボックスの初期化
		  put_colbutton_flag=FALSE;
		SetFocus(GetDlgItem(hDlg, IDOK));	// OKボタンにフォーカスを設定
		return FALSE;				// フォーカスを設定した時はFALSEを返す

	  case WM_COMMAND:				// コマンドを受け取った
		switch (wParam) {
		  case IDOK:				// [OK]ボタンが押された
		  case IDCANCEL:			// [閉じる]が選択された
			  put_colbutton_flag=TRUE;
			EndDialog(hDlg, TRUE);	// タイアログ・ボックスを閉じる
			return TRUE;
		}
		break;
	}
	return FALSE;	// メッセージを処理しなかった場合はFALSEを返す
}
/* =====================================================================
	color option
===================================================================== */

void paint(HWND hWnd, int xpos, int ypos)
{
	BYTE	col,c;
	int		i,j;

	col=get_pixel(baseBuf,xpos,ypos);
	c=pcolor;
	if (col==c) return; 

	for (j=0;j<winHeight;j++)
		for (i=0;i<winWidth;i++)
			if (get_pixel(baseBuf,i,j)==col)	set_pixel(tmpBuf,i,j,0);
			else								set_pixel(tmpBuf,i,j,1);

	undo_store(hWnd);
//	paint_core(baseBuf,xpos,ypos,c,col);
	paint_core(tmpBuf,xpos,ypos,2,0);

	for (j=0;j<winHeight;j++)
		for (i=0;i<winWidth;i++)
			if (get_pixel(tmpBuf,i,j)==2)
				if (brush_pat[i%16][j%16]==1)
					set_pixel(baseBuf,i,j,c);
				else
					set_pixel(baseBuf,i,j,bgc);
	InvalidateRect(hWnd,NULL,FALSE);
}

void draw_text(HWND hDlg,TCHAR *fname,int fsize,TCHAR *fstr,BYTE charset,BYTE bold,BYTE italic,int baseline,int align)
{
	LOGFONT		lf;
	int			i,j,x,y;
	HFONT		hFontSystem,hFont,hFontOld;
	TCHAR		*s,str[1000];
	RECT		rc;
	BYTE		c,bc;
	UINT		align_tbl[]={DT_LEFT,DT_CENTER,DT_RIGHT};
	c=pcolor;
	bc=c==3 ? 0 : 3;

	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++) {
			set_pixel(tmpBuf,x,y,3);
			set_pixel(freeareaBuf,x,y,3);
			set_pixel(copyBuf,x,y,bc);
		}

	hFontSystem = (HFONT)GetStockObject(SYSTEM_FONT);
    GetObject( hFontSystem, sizeof(LOGFONT), &lf );

	wcscpy( lf.lfFaceName, fname );
	lf.lfHeight=fsize;
	lf.lfCharSet = charset;
	lf.lfWeight = bold==FALSE ? 0 : 1000;
	lf.lfItalic = italic;
	if ( ( hFont = CreateFontIndirect( &lf ) ) != NULL ) {

		hFontOld = (HFONT)SelectObject(tmpDC, hFont);

		s=str;
		rc.left=0;
		rc.right=winWidth-10;
		rc.top=0;
		rc.bottom=winHeight;

		for (i=0;;i++) {
			if (i>=1000) break;
			*s=fstr[i];
			if ((fstr[i]==0x0d && fstr[i+1]==0x0a) || fstr[i]=='\0' || i==1000-1) {
				*s='\0';
				DrawText(tmpDC, str, -1,&rc, align_tbl[align] | DT_TOP);
				rc.top+=fsize+baseline;
				for (y=0;y<winHeight;y++)
					for (x=0;x<winWidth;x++)
						if (get_pixel(tmpBuf,x,y)!=3)
							set_pixel(copyBuf,x+5,y+5,c);			
				if (fstr[i]=='\0') break;
				s=str;
				i+=1;
			} else s++;
		}
	    SelectObject(tmpDC, hFontOld);
	    DeleteObject(hFont);
	}
/*
	for (y=0;y<winHeight;y++)
		for (x=0;x<winWidth;x++)
			if (get_pixel(tmpBuf,x,y)!=3)
				set_pixel(copyBuf,x,y,erase_flag==0 ? pcolor : pcolor_store);			
			else
				set_pixel(copyBuf,x,y,pcolor!=3 ? 3 : 0);			
*/			
	clipboard_crop(copyBuf,bc);
	
	for (j=0;j<winHeight;j++)
		for (i=0;i<winWidth;i++)
			if (region_sx<=i && i<=region_ex && region_sy<=j && j<=region_ey)
				set_pixel(freeareaBuf,i,j,1);
			else
				set_pixel(freeareaBuf,i,j,0);

	select_x=0;
	select_y=0;
	select_dx=font_x;
	select_dy=font_y;
	region_flag=DRAG;
	rect_flag=TRUE;
	clipboard_flag=TRUE;
	clipboard_save_flag=FALSE;
	make_command_bar(GetParent(hDlg));
	InvalidateRect(GetParent(hDlg),NULL,FALSE);
}

void clipboard_crop(BYTE *buf,BYTE bc)
{
	BOOL	f;
	int i,j;

	f=FALSE;
	for (i=0;i<winWidth;i++) {
		for (j=0;j<winHeight;j++) {
			if (get_pixel(buf,i,j)!=bc) {
				f=TRUE;
				break;
			}
		}
		if (f==TRUE) break;
	}
	region_sx=i;

	f=FALSE;
	for (j=0;j<winHeight;j++) {
		for (i=0;i<winWidth;i++){
			if (get_pixel(buf,i,j)!=bc) {
				f=TRUE;
				break;
			}
		}
		if (f==TRUE) break;
	}
	region_sy=j;

	f=FALSE;
	for (i=winWidth-1;i>=0;i--) {
		for (j=0;j<winHeight;j++) {
			if (get_pixel(buf,i,j)!=bc) {
				f=TRUE;
				break;
			}
		}
		if (f==TRUE) break;
	}
	region_ex=i;

	f=FALSE;
	for (j=winHeight-1;j>=0;j--) {
		for (i=0;i<winWidth;i++){
			if (get_pixel(buf,i,j)!=bc) {
				f=TRUE;
				break;
			}
		}
		if (f==TRUE) break;
	}
	region_ey=j;
	if (region_ex>=winWidth) region_ex=winWidth-1;
	if (region_ey>=winHeight) region_ey=winHeight-1;
}

void magicwand(int mx,int my)
{
	int x,y;
	BYTE c;

	c=get_pixel(baseBuf,mx,my);

	for (y=0;y<winHeight;y++) 
		for (x=0;x<winWidth;x++) 
			set_pixel(tmpBuf,x,y,get_pixel(baseBuf,x,y)==c ? 0 : 1);

	paint_core(tmpBuf,mx,my,2,0);

	for (y=0;y<winHeight;y++) 
		for (x=0;x<winWidth;x++) 
			if (get_pixel(tmpBuf,x,y)==1) 
				set_pixel(tmpBuf,x,y,0);

	for (y=0;y<winHeight;y++) 
		for (x=0;x<winWidth;x++) 
			if (get_pixel(tmpBuf,x,y)==2) {
				set_pixel(freeareaBuf,x,y,2);
				if ((x>=1           && get_pixel(tmpBuf,x-1,y  )==0) || 
					(y>=1           && get_pixel(tmpBuf,x  ,y-1)==0) ||
					(x<=winWidth-2  && get_pixel(tmpBuf,x+1,y  )==0) ||
					(y<=winHeight-2 && get_pixel(tmpBuf,x  ,y+1)==0))
					set_pixel(freeareaBuf,x,y,1);
			}
//	memcpy(baseBuf,freeareaBuf,winWidth*winHeight/4);
}		
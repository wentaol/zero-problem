﻿#pragma once

#include <wx/defs.h>

enum {
	CMD_UNKNOWN = wxID_HIGHEST + 1,

	// ファイルメニュー
	CMD_APPEND,
	CMD_DELETE,
	CMD_CLEAR,
	CMD_EXIT,

	// 問題メニュー
	CMD_GENERATE,
	CMD_STOP,
	CMD_SETTING,

	// HTML
	CMD_MERGE_HTMLS,
	CMD_SEPARATE_HTML,
	CMD_SEPARATE_HTML_BY_PROBLEM_COLOR,

	// ヘルプメニュー
	CMD_USER_GUIDE,
	CMD_LICENSE,

	// Generatorの初期化失敗/中断/終了時
	CMD_GENERATION_FAILED,
	CMD_GENERATION_CANCELLED,
	CMD_GENERATION_COMPLETED,

	// HTMLの保存に失敗
	CMD_SAVE_ERROR,

	// ボタン
	CMD_CLOSE,
	CMD_CHOOSE_PATH_LZ,
	CMD_CHOOSE_PATH_WEIGHTS,

	// HTMLの統合ダイアログ
	CMD_MD_APPEND,
	CMD_MD_DELETE,
	CMD_MD_CLEAR,
	CMD_MD_MERGE,
	CMD_MD_CLOSE
};

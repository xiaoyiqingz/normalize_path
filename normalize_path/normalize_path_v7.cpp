#include <tchar.h>

enum PATH_TYPE{
	RELITIVE_PATH = 0,
	ABSOLUTE_PATH_WITHOUT_DRIVE,             
	ABSOLUTE_PATH_WITH_DRIVE,
};

enum TOKEN_TYPE {
	TOKEN_START = 0,
	TOKEN_ONE_POINT,
	TOKEN_TWO_POINT,
	TOKEN_FILE_NAME,
};

struct token_t{
	int type;
	int path_type;
	TCHAR* begin;
	TCHAR* end;
};

struct token_t* get_next_token(struct token_t *tok)
{
	enum STAT_INNER{
		STAT_TOKEN_START = 0,
		STAT_ONE_POINT,
		STAT_TWO_POINT,
		STAT_CHAR,
		STAT_TOKEN_END,
	};

	while(*tok->end != _T('\0') &&
			(*tok->end == _T('\\') || *tok->end == _T('/'))){
		tok->end++;
	}

	int stat = STAT_TOKEN_START;
	tok->begin = tok->end;
	while (1){
		switch(stat){
			case STAT_TOKEN_START:
				if(*tok->end == _T('.')){
					stat = STAT_ONE_POINT;
				}else if(*tok->end == _T('\0')){
					return NULL;
				}else{
					stat = STAT_CHAR;
				}
				break;

			case STAT_ONE_POINT:
				if(*tok->end == _T('.')){
					stat = STAT_TWO_POINT;
				}else if(*tok->end == _T('\\') ||
						*tok->end == _T('\0') ||
						*tok->end == _T('/')){
					tok->type = TOKEN_ONE_POINT;
					stat = STAT_TOKEN_END;
				}else{
					stat = STAT_CHAR;
				}
				break;

			case TOKEN_TWO_POINT:
				if(*tok->end == _T('\\') ||
						*tok->end == _T('\0') ||
						*tok->end == _T('/')){
					tok->type = TOKEN_TWO_POINT;
					stat = STAT_TOKEN_END;
				}else{
					stat = STAT_CHAR;
				}
				break;

			default:
				if(*tok->end == _T('\\') || 
						*tok->end == _T('\0') ||
						*tok->end == _T('/')){
					tok->type = TOKEN_FILE_NAME;
					stat = STAT_TOKEN_END;
				}else{
					stat = STAT_CHAR;
				}
		}

		if (stat != STAT_TOKEN_END) {
			tok->end ++;
		} else {
			break;
		}
	}

	return tok;
}

TCHAR *go_back_previous_slash(TCHAR *write_head, TCHAR *path, int path_type)
{
	TCHAR *back_slant_index, *slash_index, *ch_index;
	if(write_head == path){
		return NULL;
	}else{
		*write_head = _T('\0');
		slash_index = _tcsrchr(path, _T('/'));
		back_slant_index = _tcsrchr(path, _T('\\'));	
		ch_index = (slash_index > back_slant_index ? 
				slash_index : back_slant_index);	//achive the separator near the tail of path

		if(ch_index == NULL){
			write_head = path;
		}else if(path_type == ABSOLUTE_PATH_WITHOUT_DRIVE &&
				ch_index == path){   //absolute path without drive
			write_head = ch_index + 1;
		}else if(path_type == ABSOLUTE_PATH_WITH_DRIVE && 
				ch_index == path + 2){ //absolute path with drive
			write_head = ch_index + 1;
		}else{
			write_head = ch_index;
		}
	}

	return write_head;
}

TCHAR *normalize_path(TCHAR *path)
{
	struct token_t path_tok, *token_ret = NULL;
	TCHAR *write_head = path;
	int len = _tcslen(path);
	int path_type;

	path_tok.begin = path_tok.end = path;
	if(path[0] == _T('\\') ||
			path[0] == _T('/')){	//absolute path without drive
		write_head++;
		path_tok.end++;
		path[0] = _T('\\');		//replace '/'
		path_type = ABSOLUTE_PATH_WITHOUT_DRIVE;
	}else if(len > 0 && path[1] == _T(':')){

		if(len > 1 && 
				path[2] != _T('\\') &&
				path[2] != _T('/')){	//error "c:" without "\"
			return NULL;
		}else if((path[0] >= _T('A') && path[0] <= _T('Z')) ||
				(path[0] >= _T('a') && path[0] <= _T('z'))){	//absolute path with drive
			write_head += 3;
			path_tok.end += 3;
			path[2] = _T('\\');		//replace '/'
			path_type = ABSOLUTE_PATH_WITH_DRIVE;
		}else{	//error "t:" not among a to z or A to Z
			path_type = RELITIVE_PATH;
		}

	}else{
		path_type = RELITIVE_PATH;
	}//start

	while(1){
		token_ret = get_next_token(&path_tok);
		if(token_ret == NULL){
			break;
		}

		switch(path_tok.type){
			case TOKEN_ONE_POINT:
				break;
			case TOKEN_TWO_POINT:
				write_head = go_back_previous_slash(write_head, path, path_type);
				if(!write_head)
					return NULL;
				break;
			case TOKEN_FILE_NAME:
				if(path_tok.begin == write_head){
					write_head = path_tok.end;
				} else {
					if(write_head != path && *(write_head - 1) != _T('\\')){
						*write_head++ = _T('\\');
					}	
					_tcsncpy(write_head, path_tok.begin,
							path_tok.end - path_tok.begin);
					write_head += path_tok.end - path_tok.begin;
				}
				break;
			default:
				return NULL;
		}
	}
	*write_head = _T('\0');

	return path;	
}

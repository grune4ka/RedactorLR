
//============>!!!!! скопировать вместо содержимого globals.h и в init написать вызов redactor();!!!!! <===============================


#ifndef _GLOBALS_H 
#define _GLOBALS_H

//--------------------------------------------------------------------
// Include Files
#include "lrun.h"
#include "web_api.h"
#include "lrw_custom_body.h"



#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define REC 1
#define IGN 0
#define END 0
#define ALL 0
#define ONL 1
#define COMMENT 1
#define SIMPREC 0

int count_com_name = -1;
struct File_stream {
	long pos;
	long f_discr;
};
typedef struct File_stream st_file;

//==================================================================================================================	

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<ДЛЯ РЕДАКТИРОВАНИЯ ПЕРЕД ЗАПУСКОМ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// номера первых транзакций которые будут в функции
int number_trans_in_fun[] = { 1, 2, 4, 12, 19 };

// номер кейса
const char *number_case = "11";

// Ищем или нет EXTRARES в метриках
#define SEARCH_EXTRARES_ON_OFF 0

char *comment_name[26];
char *name_fun_action[sizeof(number_trans_in_fun) / sizeof(int)];

// имена функций которые будут вставленны в Action()
void init_name_fun_for_action() {
	name_fun_action[0] = "void internet_bank_client()  // Отображается страница «Интернет Клиент-Банк» сайта\n{\n";
	name_fun_action[1] = "}\n\n\nvoid first_reference_news_push()  // Нажать на ссылку первой новости\n{\n";
	name_fun_action[2] = "}\n\n\nvoid demonstration_operation_in_sustem()  // Нажать «Демонстрация операций в системе Интернет Клиент-Банк»\n{\n";
	name_fun_action[3] = "}\n\n\nvoid service_DBO_for_entity_person()  // Нажать «Услуги ДБО для юридических лиц»\n{\n";
		name_fun_action[4] = "}\n\n\nvoid press_business_support()  // Нажать «Бизнес-поддержка»\n{\n";
//	name_fun_action[5] = "}\n\n\nvoid that_can_be_paid()  // нажать Что можно оплатить\n{\n";
//	name_fun_action[6] = "}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[7]  ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[8]  ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[9]  ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[10] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[11] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[12] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[13] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[14] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[15] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[16] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[17] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[18] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[19] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
//	name_fun_action[20] ="}\n\n\nvoid <!!!THIS PLACE NAME FUNCTION!!!>()  // ЛЮБОЙ КОММЕНТ\n{\n";
}

void init_name_comment() {

	comment_name[0] = "---\n";
}
	
//==================================================================================================================	




							




// сохранить позицию в файле чтения
void save_pos(st_file *fp) {
	fp->pos = ftell(fp->f_discr);
}

// перейти на сохраненную позицию
void back_pos(st_file *fp) {
	fseek(fp->f_discr, fp->pos, SEEK_SET);
}

// проверяем совпадение ряда символов, index передается по адресу

int parse_string(int *index, const int symbol, const char *str) {
	if (*index >= 0) 
		if (str[(*index)++] != symbol && str[*index = 0] == symbol) *index = 1;
		else if (*index == strlen(str)) *index = -1;
	return *index < 0 ? 1 : 0;
}

// пишет или пропускает только пробельные символы, в случае обнаружения пустой строки возвращает 1, иначе 0
// при достижении конца файла возвращает -1

int space_run(st_file *fp_from, st_file *fp_to, const int mode) {
	int sym, count = 0;
	while (isspace(sym = fgetc(fp_from->f_discr))) {
		if (sym == '\n') ++count;
		if (mode) fputc(sym, fp_to->f_discr);
	}
	if (sym >= 0) fseek(fp_from->f_discr, -1, SEEK_CUR);
	if (sym < 0) return -1;
	if (count) return 1;	
	return 0;
}

// пишет или проверяет не более одной строки, запись и возврат 1 происходит в случае совпадения str в строке,
// иначе вернет 0 и сбросит позицию на исходную в одном из файлов 
// при сount > 0 пишет или проверяет первые count символов, при достижении конца файла возвращает -1

int search_and_write_or_read(st_file *fp_from, st_file *fp_to, int count, const int mode, const char *str, ...) {
	int sym, flag = 0, index = 1, *arr;
	const char **_param_str = &str;
	while (*++_param_str) ++index;
	arr = (int*)calloc(sizeof(int) * index);
	save_pos(fp_from); save_pos(fp_to);
	if (count) ++count;
	while ((sym = fgetc(fp_from->f_discr)) >= 0 && sym != '\n' && --count) {
		if (!flag)
			for (_param_str = &str, index = 0; *_param_str; ++_param_str, ++index) 
				if (flag = parse_string(arr + index, sym, *_param_str)) 
				    break;			
		if (mode) fputc(sym, fp_to->f_discr);
	}
	free(arr);
	if (sym < 0) return -1;
	else if (mode && flag) { fputs("\n\n\t", fp_to->f_discr); return 1; }
	else if (mode && !flag) back_pos(fp_to);				// если надо писать и ненашли скидываем файл записи
	else if (!mode && !flag) back_pos(fp_from);				// если просто проверяем строку и ненашли, скидываем файл чтения
	else 
		return 1;
	return 0;
}

// пишет до первой пустой строки, в процессе записи проверяет вхождение двух строк
// str_pos - сохраняет позицию в начале этой строки, сохраняется позиция перед первым вхождением строки
// str_search - если нашли и в конце не конец файла возвращаем 1, если конец файла возвращаем -1, если не нашли вернем 0

int write_before_empty_string(st_file *fp_from, st_file *fp_to, const int mode_comment, const char *str_pos, const char *str_search) {
	int sym, flag_pos = 0, flag_search = 0, new_str_or_end_file, strlen_pos, strlen_search, i = 0;
	save_pos(fp_to);
	if (mode_comment) fputs("//", fp_to->f_discr);
	while ((sym = fgetc(fp_from->f_discr)) >= 0) {
		fputc(sym, fp_to->f_discr);
		if (++i < 20) {
			parse_string(&flag_pos, sym, str_pos);
			if (*str_search) parse_string(&flag_search, sym, str_search);
		}
		if (sym == '\n') {
			if (flag_pos >= 0) 
				save_pos(fp_to);
			if (mode_comment) fputs("//", fp_to->f_discr);	
			if (new_str_or_end_file = space_run(fp_from, fp_to, REC)) 
				break;	
			i = 0;			
		}			
	}
	if (new_str_or_end_file < 0)
		return -1;
	else if (flag_search < 0)	
		return 1;	
	return 0;
}

// ищет строку-ки до конца запроса (до первой пустой строки).

int search_in_request(st_file *fp_from, const char *str, ...) {
	int sym, index = 1, *arr, i = 0;
	const char **_param_str = &str;
	while (*++_param_str) ++index;
	arr = (int*)calloc(sizeof(int) * index);
	while ((sym = fgetc(fp_from->f_discr)) >= 0) {
		if (sym == '\n') {
			while (isspace(sym = fgetc(fp_from->f_discr)) && sym != '\n');
			if (sym == '\n') break;
			i = 0;
		}
		if (++i < 20) {
			for (_param_str = &str, index = 0; *_param_str; ++_param_str, ++index) 
				if (parse_string(arr + index, sym, *_param_str)) {
				    while ((sym = fgetc(fp_from->f_discr)) != '\n' && sym >= 0);
				    save_pos(fp_from);
				    free(arr);
					return 1;
				}
		}
	}
	free(arr);
	return sym >= 0 ? 0 : -1;
}

// проверяет строку на вхождение одного из набора символов, в случае успеха при mode != 0 записывает эту строку

int check_string_of_any_found(st_file *fp_from, st_file *fp_to, const int mode, const char *str, ...) {
	int sym, index = 1, *arr, i = 0;
	const char **_param_str = &str;
	save_pos(fp_from); 
	if (mode) save_pos(fp_to);
	while (*++_param_str) ++index;
	arr = (int*)calloc(sizeof(int) * index);
	while ((sym = fgetc(fp_from->f_discr)) != '\n' && sym >= 0) {
		for (_param_str = &str, index = 0; *_param_str; ++_param_str, ++index) 
			if (parse_string(arr + index, sym, *_param_str)) {			
			if (mode)
				back_pos(fp_from);			
				do {
					sym = fgetc(fp_from->f_discr);
					if (mode) fputc(sym, fp_to->f_discr);
					else { while ((sym = fgetc(fp_from->f_discr))!= '\n' && sym >= 0); break; }
				}while (sym != '\n' && sym >= 0);							
			    free(arr);
				return sym >= 0 ? 1 : -1;
			}
	}
	if (mode) back_pos(fp_to);
	free(arr);
	return sym >= 0 ? 0 : -1;
}

// создание и наполнения файла проверочных функций
void create_inspection(const char *count_transaction) {
	long insp;
	char content[] = "void uc_00_trans_00_Inspection()\n{\n\tnot_Found_Inspection();\n\n\tweb_reg_find(\"Text=\",\n\t\t\"SaveCount=Error_Status_1\",\n\t\tLAST);\n}\n";
	insp = fopen("_pages_inspection.h", "a+");
	lr_output_message("Создание функции в файле _page_inspection.h");
	if (count_transaction[5] == 48 && count_transaction[6] == 49)
		fputs("\nvoid not_Found_Inspection()\n{\n\tweb_reg_find(\"Search=Headers\",\n\t\t"
	             "\"SaveCount=Error_Status_404\",\n\t\t"
	             "\"Text=404 Not Found\",\n\t\t"
	             "LAST);\n}\n\n", insp);
	content[8] = *number_case;
	content[9] = number_case[1];
	content[17] = count_transaction[5];
	content[18] = count_transaction[6];	
	fputs("// ", insp);
	fputs(comment_name[count_com_name], insp);
	fputs(content, insp);
	fclose(insp);
}

// переход к следующей строке
int go_to_second_string(st_file *fp_from) {
	int sym;	
	while (isspace(sym = fgetc(fp_from->f_discr)) && sym >= 0);
	while ((sym = fgetc(fp_from->f_discr)) != '\n' && sym >= 0);
	while (isspace(sym = fgetc(fp_from->f_discr)) && sym >= 0)
		if (sym == '\n')
			go_to_second_string(fp_from);
	fseek(fp_from->f_discr, -1, SEEK_CUR);
	return sym >= 0 ? 1 : 0;	
}

// Для отладки редактора

void debug_fun(st_file *fp_from) {
	char str[100];
	long pos;
	pos = ftell(fp_from->f_discr);
	fgets(str, 100, fp_from->f_discr);
	lr_output_message("------------------------------->%s", str);
	fseek(fp_from->f_discr, pos, SEEK_SET);
}

void redactor() {
	long global_pos, file;
	int flag_1 = 0, flag_2 = 0, sym, index_numb_tran = 0, i;
	st_file fp_from, fp_to;
	char name_par_file[] = "UC00.prm", name_par_file_bak[] = "UC00.prm.bak" , str_for_numb[4];
	char name_tr[] = "UC00_00_transaction", insp_content[] = "uc_00_trans_01_Inspection();\n\n\twriteTransactionLogs(\"Transaction_Name = ";
	char name_file[] = "_uc00_lr_level.h"; 	
	init_name_fun_for_action();
	init_name_comment();	
	name_par_file_bak[2] = name_par_file[2] = name_file[3] = name_tr[2] = *number_case;
	name_par_file_bak[3] = name_par_file[3] = name_file[4] = name_tr[3] = number_case[1];
	fp_from.f_discr = fopen("Action.c", "rt");
	fp_from.pos = 0;
	fp_to.f_discr = fopen(name_file, "w+t");
	fp_to.pos = 0;
	lr_output_message("\n---\nВ процессе записи файл inspection дописывает в конец, стоит иметь ввиду\nНе забыть подключить _globals.h\n---\n");
	while (!feof(fp_from.f_discr)) {
		if (space_run(&fp_from, &fp_to, IGN) < 0)
			break;
		global_pos = ftell(fp_from.f_discr);		
		if (search_and_write_or_read(&fp_from, &fp_to, 25, IGN, "ajaxbanners.php", END) > 0) {
			while ((sym = fgetc(fp_from.f_discr)) >= 0) 
				if (sym == '\n') {
					while (isspace(sym = fgetc(fp_from.f_discr)) && sym != '\n');
					if (sym == '\n') break;				
				}
			continue;
		}		
		else if (search_and_write_or_read(&fp_from, &fp_to, 25, IGN, "start_transaction(", END) > 0) {
			lr_output_message("Редактирование lr_start_transaction");
			if (++name_tr[6] == 58) {
				name_tr[6] = 48;
				if (++name_tr[5] == 57)
					lr_error_message("Overhead name transaction");
			}
			
			for (index_numb_tran = 0; index_numb_tran < sizeof(number_trans_in_fun) / sizeof(int); ++index_numb_tran) 
				if (atoi(name_tr + 5) == number_trans_in_fun[index_numb_tran]) {
				fputs(name_fun_action[index_numb_tran], fp_to.f_discr);
				    break;
				}
			fputc('\t', fp_to.f_discr);
			insp_content[3] = name_tr[2];
			insp_content[4] = name_tr[3];
			insp_content[12] = name_tr[5];
			insp_content[13] = name_tr[6];
			fputs(insp_content, fp_to.f_discr);				
			fputs(name_tr, fp_to.f_discr);
			fputs(" Time_Start = {Log_Time_Current}\");\n\n\t", fp_to.f_discr);
			fputs("// Комментарии к операции:\n\t// ", fp_to.f_discr);
			fputs(comment_name[++count_com_name], fp_to.f_discr);
			fputs("\n\t", fp_to.f_discr);
			fputs("lr_start_transaction(\"", fp_to.f_discr);
			fputs(name_tr, fp_to.f_discr);
			fputs("\");\n\n\t", fp_to.f_discr);
			create_inspection(name_tr);	
		}
		else if (search_and_write_or_read(&fp_from, &fp_to, 25, IGN, "end_transaction(", END) > 0) {
			lr_output_message("Редактирование lr_end_transaction");
			fputs("thinkTime(\"", fp_to.f_discr);
			fputs(name_tr, fp_to.f_discr);
			fputs("\");\n\n\tlr_end_transaction(\"", fp_to.f_discr);
			fputs(name_tr, fp_to.f_discr);
			fputs("\", LR_AUTO);\n\n\twriteErrorLogs(\"Time = {Log_Time_Current}\"\n\t\t\t\" Tranzaction_Name = ", fp_to.f_discr);
			fputs(name_tr, fp_to.f_discr);
			fputs("\");\n", fp_to.f_discr);
		}
		else if (search_and_write_or_read(&fp_from, &fp_to, 0, REC, "rosbank.ru", "DOMAIN=ibank.r", END) > 0) 
			continue;
		else if (!space_run(&fp_from, &fp_to, IGN))	{			
			if (check_string_of_any_found(&fp_from, &fp_to, IGN, 
			                              "URL=http://www.rosb", "Action=http://www.rosb",
			                              "URL=https://www.rosb", "Action=https://www.rosb",
			                              "URL=https://rosb", "Action=https://rosb",
			                              "URL=http://rosb", "Action=http://rosb", "URL=https://ibank.ros",END) > 0) 
			{
				fseek(fp_from.f_discr, global_pos, SEEK_SET);
				lr_output_message("Запись запроса");
				if (write_before_empty_string(&fp_from, &fp_to, SIMPREC, "LAST);", "EXTRARES") > 0 && SEARCH_EXTRARES_ON_OFF) {
					global_pos = ftell(fp_from.f_discr); 
					lr_output_message("Ищем EXTRARES");
					while (go_to_second_string(&fp_from)) {	
						if ((flag_1 = check_string_of_any_found(&fp_from, &fp_to, IGN, 
								                              "URL=http://www.rosb", "Action=http://www.rosb",
								                              "URL=https://www.rosb", "Action=https://www.rosb",
								                              "URL=https://rosb", "Action=https://rosb",
								                              "URL=http://rosb", "Action=http://rosb", "URL=https://ibank.ros", END)) < 0) {
							back_pos(&fp_to);
							fputs("\t\tLAST);\n\n\t", fp_to.f_discr);
							break;
						}
						flag_2 = search_in_request(&fp_from, "EXTRARES", END);
						if (!flag_1 && flag_2 > 0) {
							back_pos(&fp_to);
							lr_output_message("Перенос EXTRARES из метрики");
							if (write_before_empty_string(&fp_from, &fp_to, COMMENT, "LAST);", "") < 0)
								break;
						}
						else if (flag_1 && flag_2 || flag_2 < 0) {
							back_pos(&fp_to);
							fputs("\t\tLAST);\n\n\t", fp_to.f_discr);
							break;
						}						
					}
					lr_output_message("Продолжаем редактирование");
					fseek(fp_from.f_discr, global_pos, SEEK_SET);								
				}		
			}
			else if (!space_run(&fp_from, &fp_to, IGN))
				while ((sym = fgetc(fp_from.f_discr)) >= 0) {
				if (!isspace(sym)) flag_1 = 0;
				else if (sym == '\n' && ++flag_1 > 1)
					break;
			}			
		}		
	}
	
	fseek(fp_from.f_discr, 0, SEEK_SET);
	
	file = fopen("Old_Action.c", "w+t");
	
	lr_output_message("----------------Переписываем Action");
	
	while ((sym = fgetc(fp_from.f_discr)) >= 0) 
		fputc(sym, file);

	fclose(file);
	
	
	fclose(fp_from.f_discr);
	
	fp_from.f_discr = fopen("Action.c", "w+t");
	
	fputs("Action()\n{\n\t writeIterationLogs(\"Iteration_Number = {Log_Iteration_Number}\");\n\n", fp_from.f_discr);
	flag_2 = -1;
	for (index_numb_tran = 0; index_numb_tran < sizeof(number_trans_in_fun) / sizeof(int); ++index_numb_tran) {
		fputc('\t', fp_from.f_discr);
		for (i = 0, flag_1 = 0; name_fun_action[index_numb_tran][i] != ')'; ++i) {
			if (!flag_1 && name_fun_action[index_numb_tran][i] == 'd')
				flag_1 = 1;
			else if (flag_1)
				fputc(name_fun_action[index_numb_tran][i], fp_from.f_discr);
		}
		fputs(");\t\t\t//", fp_from.f_discr);
		
		if (++flag_2 < sizeof(number_trans_in_fun) / sizeof(int)) {
			fputs(" Шаги: ", fp_from.f_discr);
			itoa(number_trans_in_fun[flag_2], str_for_numb, 10);
			fputs(str_for_numb, fp_from.f_discr);
			fputs(" - ", fp_from.f_discr);
			if (flag_2 + 1 < sizeof(number_trans_in_fun) / sizeof(int) && number_trans_in_fun[flag_2 + 1] - 1 != number_trans_in_fun[flag_2]) {
				itoa(number_trans_in_fun[flag_2 + 1] - 1, str_for_numb, 10);
				fputs(str_for_numb, fp_from.f_discr);
			}
		}
		flag_1 = 0;
		while ( name_fun_action[index_numb_tran][++i] != '\n') {
			if (!flag_1 && name_fun_action[index_numb_tran][i] == '/')
				flag_1 = 1;
			else if (flag_1)
				fputc(name_fun_action[index_numb_tran][i], fp_from.f_discr);
		}

		fputs("\n\n", fp_from.f_discr);
	}	
	fputs("\t return 0;\n}", fp_from.f_discr);
	fclose(fp_from.f_discr);
	lr_output_message("---> СОЗДАЕМ ФАЙЛ ПАРАМЕТРОВ");
	file = fopen(name_par_file, "w+t");
	fputs("[parameter:Log_Iteration_Number]\n"
		"Format=\"%d\"\n"
		"OriginalValue=\"\"\n"
		"Type=\"CurrentIteration\"\n"
		"ParamName=\"Log_Iteration_Number\"\n"
		"[parameter:Log_User_ID]\n"
		"Format=\"%s\"\n"
		"OriginalValue=\"\"\n"
		"Type=\"Userid\"\n"
		"ParamName=\"Log_User_ID\"\n"
		"[parameter:Log_Load_Generator_Name]\n"
		"Format=\"%s\"\n"
		"OriginalValue=\"\"\n"
		"Type=\"Host\"\n"
		"ParamName=\"Log_Load_Generator_Name\"\n"
		"[parameter:Log_Time_Current]\n"
		"Format=\"%H:%M:%S\"\n"
		"ParamName=\"Log_Time_Current\"\n"
		"GenerateNewVal=\"EachOccurrence\"\n"
		"Type=\"Time\"\n"
		"OriginalValue=\"\"\n"
		"OffsetUI=\"0\"\n"
		"Offset=\"\"\n"
		"WorkingDaysOn=\"0\"\n", file);

	fclose(file);

	file = fopen(name_par_file_bak, "w+t");
	fputs("[parameter:Log_Time_Currents]\n"
		"Format=\"%H:%M:%S\"\n"
		"ParamName=\"Log_Time_Currents\"\n"
		"GenerateNewVal=\"EachOccurrence\"\n"
		"Type=\"Time\"\n"
		"OriginalValue=\"\"\n"
		"OffsetUI=\"0\"\n"
		"WorkingDaysOn=\"0\"\n"
		"Offset=\"\"\n"
		"[parameter:Log_Iteration_Number]\n"
		"Format=\"%d\"\n"
		"Type=\"CurrentIteration\"\n"
		"OriginalValue=\"\"\n"
		"ParamName=\"Log_Iteration_Number\"\n"
		"[parameter:Log_User_ID]\n"
		"Format=\"%s\"\n"
		"Type=\"Userid\"\n"
		"OriginalValue=\"\"\n"
		"ParamName=\"Log_User_ID\"\n"
		"[parameter:Log_Load_Generator_Name]\n"
		"Format=\"%s\"\n"
		"Type=\"Host\"\n"
		"OriginalValue=\"\"\n"
		"ParamName=\"Log_Load_Generator_Name\"\n"
		"[parameter:Log_Time_Current]\n"
		"Format=\"%H:%M:%S\"\n"
		"ParamName=\"Log_Time_Current\"\n"
		"GenerateNewVal=\"EachOccurrence\"\n"
		"Type=\"Time\"\n"
		"OriginalValue=\"\"\n"
		"OffsetUI=\"0\"\n"
		"WorkingDaysOn=\"0\"\n"
		"Offset=\"\"", file);
	fclose(file);

			
	lr_output_message("Переписываем init....");
	file = fopen("vuser_init.c", "w+t");
	

	fputs("// Описание бизнес-операции\t:\n"
	"// Сценарий\t\t\t\t\t: UC\n"
	"// Дата создания скрипта\t:\n"
	"// Автор скрипта\t\t\t: Рыбаков Юрий\n\n"
	"vuser_init()\n{\n\tsetLogging(LOG_RUNTIME);\n\n\t"	
	"writeStartLogs(\" Stand_Name =\"\n" 
	"\t\t\t\t\t\" Link =\"\n"
	"\t\t\t\t\t\" Login =\"\n" 
	"\t\t\t\t\t\" Password =\"\n"
	"\t\t\t\t\t\" User_ID = {Log_User_ID}\"\n" 
	"\t\t\t\t\t\" Load_Generator_Name = {Log_Load_Generator_Name}\");\n\n\t"	
	"webSetSocketsOption();\n\n\t"	
	"initializationErrorStatus();\n\n\t"	
	"initializationParam();\n\n\t"	
	"return 0;\n}\n\t", file);
	
	fclose(file);
	lr_output_message("Переписываем end");
	file = fopen("vuser_end.c", "w+t");
	fputs("vuser_end()\n{\n\twriteEndLogs(\"Completion_Time = {Log_Time_Current} Shutdown_Status = {Shutdown_Status}\");\n\n\treturn 0;\n}", file);
	fputs("\n}\n\n\n--------------The end))))\n\n\n", fp_to.f_discr);
	
	fclose(file);
	lr_output_message("Переписываем globals.h");
	file = fopen("globals.h", "w+t");
	
	fputs("#ifndef _GLOBALS_H\n"
"#define _GLOBALS_H\n\n"
"//--------------------------------------------------------------------\n"
"// Include Files\n"
"#include \"lrun.h\"\n"
"#include \"web_api.h\"\n"
"#include \"lrw_custom_body.h\"\n"
"#include \"_global.h\"\n"
"#include \"_pages_inspection.h\"\n"
"#include \"_uc", file); 
	fputs(number_case, file);
	
	fputs("_lr_level.h\"\n\n"
"//--------------------------------------------------------------------\n"
"// Global Variables\n\n"
"#endif // _GLOBALS_H\n", file);	
	fclose(file);
	


	fclose(fp_to.f_discr);
	lr_output_message("Редакция завершенна");
	lr_abort();
}



//--------------------------------------------------------------------
// Global Variables


#endif // _GLOBALS_H













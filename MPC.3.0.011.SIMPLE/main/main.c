/*
	MIDletPascal-compiler
	MPC 3.0.010 - 03.07.2010
*/
#pragma warning (disable: 4996)
#include <stdio.h>
#include "../util/strings.h"
#include "../util/memory.h"
#include "../util/error.h"
#include <string.h>


extern FILE *yyin;
//extern string* build_configuration_define; /* used in lex scanner by the preprocessor; contains the name of the build configuration */
extern string *string_constant;
extern short int error_count;
extern short int warning_count;
extern int usesForms; /* this is set to 1 if F.class must be include in the generated JAR file */
extern int usesSupport;
extern int usesFloat;
extern int usesRecordStore;
extern int usesHttp;
extern int usesPlayer;
extern int usesSMS;
extern int constant_pool_size;
extern long int last_error_linenum;
extern int inside_loop;
extern int linenum;
extern string *string_constant;
extern int warning_101_count;
extern int next_record_ID;
extern int compiling_unit;

char msgstr[1024];
char *library_directory;
char *build_configuration;
int canvasType;
int mathType;
int lowText;
//int next_record_ID;
int detetct_units_only;
char **units;
int goverify;

char *output_path;
char *source_file_name;
char *library_directory;
char *user_libraries;
char **units;
int detect_units_only; //***
char rec_file[256];

void compileMSG()
{
	fprintf(stdout /*stderr*/, (const char *) &msgstr);
};


void compile_progress(int x)
{
	sprintf(msgstr,"\r(%d), %d ", linenum, x); compileMSG();
};


void compile_terminate()
{
	mem_close();
	fclose(yyin);
	exit(1);
};

//////////

void initialize_compiler()
{
	constant_pool_size = 0;
	string_constant = NULL;
	warning_101_count = 0;
	error_count = 0;
	warning_count = 0;
	last_error_linenum = -1;
	linenum = 1;
	inside_loop = 0;
	lowText = 1;
	goverify = 1;
	mathType = 1;
	canvasType = 0; //NORMAL;
	usesFloat = 0;
	usesForms = 0;
	usesSupport = 0;
	usesRecordStore = 0;
	usesHttp = 0; 
	usesPlayer = 0;
	//usesRegisteredFeature = 0;
	usesSMS = 0;
	user_libraries = NULL;
}

int main(int argc, char **argv)
{
/*extern compile_result __cdecl compiler_run(char* buildFile,
				  char *library_directory_arg,
				  char *build_configuration,
				  void (*compile_terminate_arg)(),
				  void (*compile_write2_arg)(char*, char*),
				  void (*compile_write3_arg)(char*, char*, char*),
				  void (*compile_progress_arg)(int),
				  int canvasType_arg,
				  int mathType_arg,
				  int next_record_ID_arg,
				  int detetct_units_only_arg,
				  char **units_arg)
{
*/

	FILE *fp;	
	char source_file[1024];
	char output_directory[1024];
	//char final_message[1024];
	int pos;  //, len;
	//int testVar;
	sprintf(msgstr,"MIDletPascal compiler. v.3.0.011 beta (030810)\nNiksa Orlic (c).  http://sourceforge.net/projects/midletpascal/\n");
	compileMSG();
	if (argc >= 2)
	{
		/*
		units = units_arg;
		detect_units_only = detetct_units_only_arg;
		canvasType = canvasType_arg;
		mathType = mathType_arg;
		next_record_ID = next_record_ID_arg;
		library_directory = library_directory_arg;
		*/
		detect_units_only =0;
		mem_init();
		initialize_compiler();
		units=mem_alloc(8);
		*units=mem_alloc(8);
		*units=NULL;
		fp = fopen(argv[1], "r");
		if (fp == NULL) die(5);
		fgets(source_file, 1024, fp);
		fgets(output_directory, 1024, fp);
		fclose(fp);
		pos = (int) strlen(source_file) - 1;
		while ((source_file[pos] == '\r') || (source_file[pos] == '\n')) pos --;
		source_file[pos + 1] = '\0';
		source_file_name = (char*) mem_alloc(pos + 1);
		if (source_file_name == NULL)  die(1);
		//pos = len - 1;
		while ((pos >= 0) && (source_file[pos] != '\\'))  pos --;
		if (source_file[pos] != '\\')  die(21);
		//pos ++;
		strcpy(source_file_name, source_file + pos + 1);
		////////////////////
		pos = (int) strlen(output_directory) - 1;
		while ((output_directory[pos] == '\r') || (output_directory[pos] == '\n')) pos --;
		output_directory[pos + 1] = '\0';
		//len = strlen(output_directory);	output_path = (char*) mem_alloc(len + 1);
		output_path = output_directory;
		library_directory=output_directory;
        ///
		next_record_ID=0;
		while (next_record_ID<100)
		{
          sprintf(rec_file, "%s\\R_%d.class", output_directory, next_record_ID);
          fp = fopen(rec_file, "r");
		  if (fp == NULL) break;
		  fclose(fp);
		  next_record_ID++;
		}
		///
		yyrestart(source_file);
		if ((yyin != NULL) && (output_path != NULL))
		{
			//if (build_configuration != NULL) build_configuration_define = string_from_cstr(build_configuration);
			//else build_configuration_define = string_from_cstr("");
			//build_configuration_define = string_from_cstr("");
			//lowercase(string_get_cstr(build_configuration_define));
			sprintf(msgstr,"Compiling \'%s\'...\n", source_file_name);
			compileMSG();
			strcpy(output_path, output_directory);
			parser_start();
			fclose(yyin);
			mem_free(output_path);
			mem_free(source_file_name);
		}
	}
	if (string_constant != NULL) string_destroy(string_constant);
	sprintf(msgstr,"\rDone - %d error(s), %d warning(s)\n", error_count, warning_count);
	compileMSG();
	if (error_count == 0)
	{
		if (compiling_unit == 0)
		{
			if (canvasType == 0) sprintf(msgstr,"<<Canvas: NORMAL\n");
			if (canvasType == 1) sprintf(msgstr,"<<Canvas: FULL_MIDP20\n");
			if (canvasType == 2) sprintf(msgstr,"<<Canvas: FULL_NOKIA\n");
			compileMSG();
		}
		if (usesForms)
		{
			sprintf(msgstr,"<<FS.class\n");
			compileMSG();
		}
		if (usesSupport)
		{
			sprintf(msgstr,"<<S.class\n");
			compileMSG();
		}
		if (usesFloat & 1)
		{
			sprintf(msgstr,"<<Float1.class\n");
			compileMSG();
		}
		if (usesFloat & 2)
		{
			sprintf(msgstr,"<<Float2.class\n");
			compileMSG();
		}
		if (usesRecordStore == 1)
		{
			sprintf(msgstr,"<<RS.class\n");
			compileMSG();
		}
		if (usesHttp == 1)
		{
			sprintf(msgstr,"<<H.class\n");
			compileMSG();
		}
		if (usesPlayer == 1)
		{
			sprintf(msgstr,"<<P.class\n");
			compileMSG();
		}
		if (usesSMS == 1)
		{
			sprintf(msgstr,"<<SM.class\n");
			compileMSG();
		}
	}
	mem_close();
    return 0;
}
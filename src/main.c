#include <gtk/gtk.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>

#define	PROGRESS_SIZE	102400	//100k
#define	ERASER_SIZE		512	//1k
#define	ERASER_ENC_SIZE		896	//1k

/*- Log define ------------*/
char	*LogName;
#define LOGFILE LogName
#define ERR LOGFILE,1,__LINE__
#define INF LOGFILE,2,__LINE__
#define WRN LOGFILE,3,__LINE__
#define DBG LOGFILE,4,__LINE__

#define DEF "./bxre.log",1,__LINE__

GtkBuilder	*builder; 
GtkWidget	*window;
GtkWidget 	*g_lbl_status;
GtkWidget 	*g_bar_progress;

void on_window_main_destroy();
void on_smn_open_activate (GtkMenuItem *smn_open, gpointer *data);
void on_smn_quit_activate (GtkMenuItem *smn_quit, gpointer *data);
void on_smn_log_activate (GtkMenuItem *smn_log, gpointer *data);
void on_smn_about_activate (GtkMenuItem *smn_about, gpointer *data); 

void on_btn_fileopen_clicked (GtkButton *btn_fileopen, gpointer *data);
void on_btn_easy_clicked (GtkButton *btn_easy, gpointer *data );
void on_btn_about_ok_clicked (GtkButton *btn_about_ok, gpointer *data); 

int func_gtk_dialog_modal(int type, GtkWidget *window, char *message);
int func_file_eraser_easy(char *filename);
int func_file_eraser(int type, char *filename);
int func_file_eraser_enc(char *filename);

int BXLog(const char *, int , int , const char *, ...);

void on_smn_about_activate (GtkMenuItem *smn_log, gpointer *data)
{
    gtk_widget_show(GTK_WIDGET(data));                
	return;
}

void on_smn_log_activate (GtkMenuItem *smn_log, gpointer *data)
{
	func_gtk_dialog_modal(0, window, "\n    현재는 해당 기능을 지원하지 않습니다.    \n");

	return;
}

void on_smn_open_activate (GtkMenuItem *smn_open, gpointer *data)
{
	on_btn_fileopen_clicked(GTK_BUTTON(data), (gpointer)window);
}

void on_btn_fileopen_clicked (GtkButton *btn_fileopen, gpointer *data)
{

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, 
			("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

    gtk_widget_show_all(dialog);

//  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/");
//	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_temp_dir());
    gint resp=gtk_dialog_run(GTK_DIALOG(dialog));

    if( resp == GTK_RESPONSE_ACCEPT)
	{
		gtk_button_set_label(GTK_BUTTON(btn_fileopen), 
					gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
   	} 
    	
	gtk_widget_destroy(dialog);

	return;
}

// called when button is clicked
void on_smn_quit_activate (GtkMenuItem *smn_quit, gpointer *data)
{
	if( func_gtk_dialog_modal( 1, GTK_WIDGET(data), "\n    정말 종료 하시겠습니까?    \n") == TRUE )
		on_window_main_destroy();

	return;
}

void on_btn_about_ok_clicked (GtkButton *btn_about_ok, gpointer *data)
{
    gtk_widget_hide(GTK_WIDGET(data));                
}

// called when button is clicked
void on_btn_easy_clicked (GtkButton *btn_easy, gpointer *data )
{
	char	filename[1024];
	char	message[1024];

	if( gtk_button_get_label(GTK_BUTTON(data)) != NULL )
		sprintf( filename, "%s", gtk_button_get_label(GTK_BUTTON(data)));
	else
		filename[0] = 0x00;

	if( filename[0] == 0x00 )
	{
		func_gtk_dialog_modal(0, window, "\n    대상파일이 선택되지 않았습니다.    \n");
	}
	else
	{
		sprintf( message, 
			"\n    삭제 후에 복구가 불가능 합니다.\n    아래 파일을 삭제하시겠습니까?\n    [ %s ]    \n", filename);
    
		if( func_gtk_dialog_modal(1, window, message) == GTK_RESPONSE_ACCEPT)
		{
			func_file_eraser_easy(filename);
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Cancelled...");
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 0 );
		}
	}
}

// called when button is clicked
void on_btn_3pass_clicked (GtkButton *btn_3pass, gpointer data )
{
	char	filename[1024];
	char	message[1024];

	if( gtk_button_get_label(GTK_BUTTON(data)) != NULL )
		sprintf( filename, "%s", gtk_button_get_label(GTK_BUTTON(data)));
	else
		filename[0] = 0x00;

	if( filename[0] == 0x00 )
	{
		func_gtk_dialog_modal(0, window, "\n    대상파일이 선택되지 않았습니다.    \n");
	}
	else
	{
		sprintf( message, 
			"\n    삭제 후에 복구가 불가능 합니다.\n    아래 파일을 삭제하시겠습니까?\n    [ %s ]    \n", filename);
    
		if( func_gtk_dialog_modal(1, window, message) == GTK_RESPONSE_ACCEPT)
		{
			func_file_eraser(3, filename);
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Cancelled...");
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 0 );
		}
	}
}

// called when button is clicked
void on_btn_7pass_clicked (GtkButton *btn_7pass, gpointer data )
{
	char	filename[1024];
	char	message[1024];

	if( gtk_button_get_label(GTK_BUTTON(data)) != NULL )
		sprintf( filename, "%s", gtk_button_get_label(GTK_BUTTON(data)));
	else
		filename[0] = 0x00;

	if( filename[0] == 0x00 )
	{
		func_gtk_dialog_modal(0, window, "\n    대상파일이 선택되지 않았습니다.    \n");
	}
	else
	{
		sprintf( message, 
			"\n    삭제 후에 복구가 불가능 합니다.\n    아래 파일을 삭제하시겠습니까?\n    [ %s ]    \n", filename);
    
		if( func_gtk_dialog_modal(1, window, message) == GTK_RESPONSE_ACCEPT)
		{
			func_file_eraser(7, filename);
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Cancelled...");
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 0 );
		}
	}
}

// called when button is clicked
void on_btn_encrypt_clicked (GtkButton *btn_encrypt, gpointer data )
{
	char	filename[1024];
	char	message[1024];

	if( gtk_button_get_label(GTK_BUTTON(data)) != NULL )
		sprintf( filename, "%s", gtk_button_get_label(GTK_BUTTON(data)));
	else
		filename[0] = 0x00;

	if( filename[0] == 0x00 )
	{
		func_gtk_dialog_modal(0, window, "\n    대상파일이 선택되지 않았습니다.    \n");
	}
	else
	{
		sprintf( message, 
			"\n    삭제 후에 복구가 불가능 합니다.\n    아래 파일을 삭제하시겠습니까?\n    [ %s ]    \n", filename);
    
		if( func_gtk_dialog_modal(1, window, message) == GTK_RESPONSE_ACCEPT)
		{
			func_file_eraser_enc(filename);
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Canceled...");
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 0 );
		}
	}
}

// called when button is clicked
void on_btn_trim_clicked()
{
	BXLog( DBG, "btn_trim_clicked...\n");
	func_gtk_dialog_modal(0, window, "\n    현재는 해당 기능을 지원하지 않습니다.    \n");
}

// called when button is clicked
void on_btn_temp_clicked()
{
	BXLog( DBG, "btn_temp_clicked...\n");
	func_gtk_dialog_modal(0, window, "\n    현재는 해당 기능을 지원하지 않습니다.    \n");
}

// called when button is clicked
void on_btn_setting_clicked()
{
	BXLog( DBG, "btn_setting_clicked...\n");
	func_gtk_dialog_modal(0, window, "\n    현재는 해당 기능을 지원하지 않습니다.    \n");
}

int main(int argc, char *argv[])
{
   	LogName = basename(argv[0]); 
	gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    // get pointers to the two labels
    g_lbl_status = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_status"));
    g_bar_progress = GTK_WIDGET(gtk_builder_get_object(builder, "bar_progress"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

// gtk_dialog_modal
int func_gtk_dialog_modal(int type, GtkWidget *widget, char *message)
{
	GtkWidget *dialog, *label, *content_area;
	GtkDialogFlags flags = GTK_DIALOG_MODAL;
	int	rtn = GTK_RESPONSE_REJECT;

	switch(type)
	{
		case 0 :
			dialog = gtk_dialog_new_with_buttons("Dialog", GTK_WINDOW(widget), flags, 
						("_OK"), GTK_RESPONSE_ACCEPT, NULL );
			break;

		case 1 :
			dialog = gtk_dialog_new_with_buttons("Dialog", GTK_WINDOW(widget), flags,
						("_OK"), GTK_RESPONSE_ACCEPT, 
						("_Cancel"), GTK_RESPONSE_REJECT, NULL );
			break;

		default :
			break;
	}

	label=gtk_label_new(message);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	gtk_container_add (GTK_CONTAINER (content_area), label);
	gtk_widget_show_all(dialog);

	rtn = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return(rtn);	
}

int func_file_eraser_easy(char *filename)
{
	FILE *fp;	
	struct   stat  file_info;
	int mode = R_OK | W_OK;
	char message[1024];
	gdouble percent = 0.0;
	gdouble size = 0.0;
	char *msize;

	memset( message, 0x00, sizeof(message));
	
	gtk_label_set_text(GTK_LABEL(g_lbl_status), "Eraser Start...");

	if( access( filename, mode ) != 0 )
	{
		func_gtk_dialog_modal(0, window, "\n    파일이 삭제 가능한 상태가 아닙니다.    \n");
	}
	else
	{
		if( stat( filename, &file_info) > 0 )
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Checking File Errors...");
		}
		else
		{


			msize = malloc(ERASER_SIZE);
			fp = fopen(filename, "w");
			sprintf( message, "file size [%ld]", file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
			
			memset( message, 0x00, strlen(message));
			sprintf( message, "%.0f%% Complete", percent);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);

			size = 0;
//			BXLog( DBG, "Target File Eraser Start : [%d/%s]\n", 0, filename );
			BXLog( DBG, "Target File Eraser Start : [%s]\n", filename );
			
			memset( msize, 0x00, ERASER_SIZE );

			while (size<file_info.st_size)
			{   
				while (gtk_events_pending ()) 
					gtk_main_iteration (); 

//				BXLog( DBG, "fp[%d], size[%f], file_size[%ld], percent:[%f]\n", fp, size, file_info.st_size, percent );

				fwrite( msize, 1, ((file_info.st_size/size))>0?ERASER_SIZE:(file_info.st_size%ERASER_SIZE), fp);
				size += ERASER_SIZE;
				percent = size/file_info.st_size*100.0;
				if( (int)size % PROGRESS_SIZE == 0 )
				{
					memset( message, 0x00, strlen(message));
					sprintf( message, "%.0f%% Complete", percent);
					gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
					gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);


					memset( message, 0x00, strlen(message));
					sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
					gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
				}
			}
			
//			BXLog( DBG, "Target File Eraser E n d : [%d/%s]\n\n", 0, filename );
			
			sprintf( message, "%.0f%% Complete", 100.0);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);
	
			fclose(fp);
			free(msize);
			memset( message, 0x00, strlen(message));
			sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
		}
	}
	
	BXLog( DBG, "Target File Eraser E n d : [%s]\n\n", filename );
//	func_gtk_dialog_modal(0, window, "\n    확인 후 삭제가 진행됩니다.    \n");
	remove( filename );
	func_gtk_dialog_modal(0, window, "\n    삭제가 완료되었습니다.    \n");
	
	return( TRUE );
}


int func_file_eraser(int type, char *filename)
{
	FILE *fp;	
	struct   stat  file_info;
	int mode = R_OK | W_OK;
	char message[1024];
	gdouble percent = 0.0;
	gdouble size = 0.0;
	char MsgTmp[5];
	char *msize;

	memset( message, 0x00, sizeof(message));
	
	gtk_label_set_text(GTK_LABEL(g_lbl_status), "Start Eraser...");

	if( access( filename, mode ) != 0 )
	{
		func_gtk_dialog_modal(0, window, "\n    파일이 삭제 가능한 상태가 아닙니다.    \n");
	}
	else
	{
		if( stat( filename, &file_info) > 0 )
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Checking File Errors...");
		}
		else
		{
			msize = malloc(ERASER_SIZE);
			fp = fopen(filename, "w");
			sprintf( message, "file size [%ld]", file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
			
			memset( message, 0x00, strlen(message));
			sprintf( message, "%.0f%% Complete", percent);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);

			BXLog( DBG, "Target File Eraser Start : [%s]\n", filename );

			for( int i=0 ; i < type ; i++ )
			{
				size = 0;
//				BXLog( DBG, "size, st_size, i, type   : [%f/%ld/%d/%d]\n", size, file_info.st_size, i, type );
//				BXLog( DBG, "Target File Eraser Start : [%d/%s]\n", i+1, filename );
				while (size<file_info.st_size)
				{   
					switch(i)
					{
						case 0 :
							MsgTmp[0] = 'A';
							memset( msize, MsgTmp[0], ERASER_SIZE );
							break;
						case 1 :
							MsgTmp[0] = '^';
							memset( msize, MsgTmp[0], ERASER_SIZE );
							break;
						case 2 :
							srand(time(NULL));
							if( size < ERASER_SIZE )
								for( int j=0 ; j < ERASER_SIZE ; j++ )
									msize[j] = 'A' + (random() % 26);
							break;
						case 3 :
							MsgTmp[0] = 'Z';
							memset( msize, MsgTmp[0], ERASER_SIZE );
							break;
						case 4 :
							MsgTmp[0] = 'A';
							memset( msize, MsgTmp[0], ERASER_SIZE );
							break;
						case 5 :
							MsgTmp[0] = '^';
							memset( msize, MsgTmp[0], ERASER_SIZE );
							break;
						case 6 :
							srand(time(NULL));
							if( size < ERASER_SIZE )
								for( int j=0 ; j < ERASER_SIZE ; j++ )
									msize[j] = 'A' + (random() % 26);
							break;
						default :
							break;

					}

					while (gtk_events_pending ()) 
						gtk_main_iteration (); 

					fwrite( msize, 1, ((file_info.st_size/size))>0?ERASER_SIZE:(file_info.st_size%ERASER_SIZE), fp);
					size += ERASER_SIZE;
					percent = (size+(file_info.st_size*i))/(file_info.st_size*type)*100.0;
					if( (int)size % PROGRESS_SIZE == 0 )
					{
						memset( message, 0x00, strlen(message));
								sprintf( message, "%.0f%% Complete", percent);
					//				gchar *message = g_strdup_printf ("%.0f%% Complete", percent);
						gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
						gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);


						memset( message, 0x00, strlen(message));
						sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
						gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
					}
				}
				
//				BXLog( DBG, "Target File Eraser E n d : [%d/%s]\n\n", i+1, filename );
				
//				func_gtk_dialog_modal(0, window, "\n   1 pass 삭제가 진행완료 되었습니다.    \n");
				fseek( fp, 0L, SEEK_SET );
			}

			sprintf( message, "%.0f%% Complete", 100.0);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);
	
			fclose(fp);
			free(msize);
			memset( message, 0x00, strlen(message));
			sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
		}
	}

	BXLog( DBG, "Target File Eraser E n d : [%s]\n\n", filename );
//	func_gtk_dialog_modal(0, window, "\n    확인 후 삭제가 진행됩니다.    \n");
	remove( filename );
	func_gtk_dialog_modal(0, window, "\n    삭제가 완료되었습니다.    \n");
	
	return( TRUE );
}

int func_file_eraser_enc(char *filename)
{
	FILE *fp;	
	struct   stat  file_info;
	int mode = R_OK | W_OK;
	char message[1024];
	gdouble percent = 0.0;
	gdouble size = 0.0;
	char MsgTmp[ERASER_ENC_SIZE];
	char *msize;

	memset( message, 0x00, sizeof(message));
	
	gtk_label_set_text(GTK_LABEL(g_lbl_status), "Start Eraser...");

	if( access( filename, mode ) != 0 )
	{
		func_gtk_dialog_modal(0, window, "\n    파일이 삭제 가능한 상태가 아닙니다.    \n");
	}
	else
	{
		if( stat( filename, &file_info) > 0 )
		{
			gtk_label_set_text(GTK_LABEL(g_lbl_status), "Checking File Errors...");
		}
		else
		{
			msize = malloc(ERASER_ENC_SIZE);
			fp = fopen(filename, "w");
			sprintf( message, "file size [%ld]", file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
			
			memset( message, 0x00, strlen(message));
			sprintf( message, "%.0f%% Complete", percent);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);

			BXLog( DBG, "Target File Eraser Start : [%s]\n", filename );

			for( int i=0 ; i < 2 ; i++ )
			{
				size = 0;
//				BXLog( DBG, "Target File Eraser Start : [%d/%s]\n", i+1, filename );
				
				if( i == 0 && size == 0 )
				{
					size = sprintf( MsgTmp, "Blue X-ray Encrypte v1.0" );
					fwrite( MsgTmp, 1, size, fp);
				}

				while (size<file_info.st_size)
				{   
					switch(i)
					{
						case 0 :
							srand(time(NULL));
							if( size < ERASER_ENC_SIZE )
								for( int j=0 ; j < ERASER_ENC_SIZE ; j++ )
									msize[j] = ' ' + (random() % 127);

							break;
						case 1 :
							MsgTmp[0] = 0x00;
							memset( msize, MsgTmp[0], ERASER_ENC_SIZE );
							break;
						default :
							break;
					}

					while (gtk_events_pending ()) 
						gtk_main_iteration (); 

					fwrite( msize, 1, ((file_info.st_size/size))>0?ERASER_ENC_SIZE:(file_info.st_size%ERASER_ENC_SIZE), fp);
					size += ERASER_ENC_SIZE;
					percent = (size+(file_info.st_size*i))/(file_info.st_size*2)*100.0;
					if( (int)size % PROGRESS_SIZE == 0 )
					{
						memset( message, 0x00, strlen(message));
						sprintf( message, "%.0f%% Complete", percent);
						gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), percent / 100.0);
						gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);

						memset( message, 0x00, strlen(message));
						sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
						gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
					}
				}
				
//				BXLog( DBG, "Target File Eraser E n d : [%d/%s]\n\n", i+1, filename );

//				func_gtk_dialog_modal(0, window, "\n    1 pass 삭제가 진행완료 되었습니다.    \n");
				fseek( fp, 0L, SEEK_SET );
			}

			sprintf( message, "%.0f%% Complete", 100.0);
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(g_bar_progress), 100.0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR(g_bar_progress), message);
	
			fclose(fp);
			free(msize);
			memset( message, 0x00, strlen(message));
			sprintf( message, "file size [%.0f byte/%ld byte]\n", size, file_info.st_size);
			gtk_label_set_text(GTK_LABEL(g_lbl_status), message);
		}
	}
	
	
	BXLog( DBG, "Target File Eraser E n d : [%s]\n\n", filename );
//	func_gtk_dialog_modal(0, window, "\n    확인 후 삭제가 진행됩니다.    \n");
	remove( filename );
	func_gtk_dialog_modal(0, window, "\n    삭제가 완료되었습니다.    \n");

	return( TRUE );
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int BXLog(const char *logfile, int logflag, int logline, const char *fmt, ...)
{
    int fd, len;
    struct  timeval t;
    struct tm *tm;
    static char fname[128];
    static char sTmp[1024*2], sFlg[5];

    va_list ap;

    switch( logflag )
    {
        case    1 :
            sprintf( sFlg, "E" );
            break;
        case    2 :
            sprintf( sFlg, "I" );
            break;
        case    3 :
            sprintf( sFlg, "W" );
            break;
        case    4 :
        default   :
#ifndef _BXDBG
            return 0;
#endif
            sprintf( sFlg, "D" );

            break;
    }

    memset( sTmp, 0x00, sizeof(sTmp) );
    gettimeofday(&t, NULL);
    tm = localtime(&t.tv_sec);

    /* [HHMMSS ssssss flag __LINE__] */
    len = sprintf(sTmp, "[%5d:%08x/%02d:%02d:%02d.%03ld/%s:%4d]",
            getpid(), (unsigned int)pthread_self(),
            tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec/1000,
            sFlg, logline );

    va_start(ap, fmt);
    vsprintf((char *)&sTmp[len], fmt, ap);
    va_end(ap);

	sprintf(fname, "%s.%02d%02d", logfile, tm->tm_mon+1, tm->tm_mday);
    if (access(fname, 0) != 0)
        fd = open(fname, O_WRONLY|O_CREAT, 0660);
    else
        fd = open(fname, O_WRONLY|O_APPEND, 0660);

    if (fd >= 0)
    {
        write(fd, sTmp, strlen(sTmp));
        close(fd);
    }

    return 0;

}/* End of BXLog() */


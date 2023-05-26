/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 02:11:39 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/21 03:09:10 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <time.h>
# include <termios.h>
# include <dirent.h>
# include "../libft/Mandatory/libft.h"

# define HEREDOC_NAME "/tmp/.minishell_heredoc"
# define COMMAND_WITH_DASH 1
# define COMMAND_ALONE 0

# define DEFAULT "\033[0m"
# define YELLOW "\033[0;33m"
# define BOLD "\033[1m"

extern int	g_last_exit_code;

/*
* types of possible tokens
* word ex: ls
* pipe: |
* input_redirect: <
* output_redirect: >
* heredoc: <<
* append: >>
*/
enum	e_token_types{
	WORD = 1,
	VARIABLE,
	PIPE,
	INPUT_REDIRECT,
	OUTPUT_REDIRECT,
	HEREDOC,
	APPEND,
	END
};

enum	e_Q_status{
	WORKING = 9,
	SQUOTE,
	DQUOTE,
};

enum	e_VAR_PARTS
{
	VAR= 1,
	VALUE = 2,
};

enum	e_FILE_TYPES
{
	IN_FILE = 1,
	OUT_FILE = 2,
};

/*
* POSSIBLE TYPES: IN_FILE OUT_FILE
*/
typedef struct s_file
{
	int				fd;
	int				type;
	struct s_file	*next;
	char			*delim;
}t_file_o;

/* 
* if bool piped == true, the output of the command 
* will be piped to the next command
* path contain path where the executable has been found
 *(if not found, path will == NULL)
*/

typedef struct s_command
{
	char				*command;
	char				**arguments;
	char				*path;
	bool				piped;
	t_file_o			**files;
	int					in_fd;
	int					out_fd;
	struct s_command	*next;
	struct s_command	*prev;
}t_command;

/*
* a token is a node of a linked list
* a token is the smallest considered unit in the shell
* it's either a word or an operator
*/
typedef struct s_token
{
	char			*content;
	int				type;
	bool			quoted;
	struct s_token	*next;
	struct s_token	*prev;
}t_token;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}t_env;

typedef struct s_garbage
{
	int		i;
	int		j;
	char	*temp_pwd;
	int		flag;
}t_garbage;

/* main structure */
typedef struct s_exec
{
	int		i;
	int		num_cmds;
	int		num_pipes;
	int		fd;
	int		**pfd;
	pid_t	*pid;
	pid_t	one_cmd_pid;
}t_exec;

typedef struct s_data
{
	char		*user_input;
	char		*working_dir;
	char		*old_working_dir;
	char		**env;
	t_env		**head_env;
	pid_t		pid;
	t_token		**head_of_tokens;
	t_command	**head_of_commands;
	char		*script_n;
	bool		ctrl_c;
	t_exec		*exec;
}t_data;

/* Execution vars */
// bool	check_type_validity(t_data	*data, int ac, char	**av);
// bool	usage_error();
void		exit_shell(t_data *data, int exit_status);

/*ENV MANIPULATION*/
int			env_variable_count(char	**env);
bool		init_data(t_data *data, char **env);
bool		init_wds(t_data*data, char	**env);
bool		ft_isspace(char c);
void		run_mini(t_data	*data);
/* PARSING */
int			type_of_token(char	*s);
bool		ft_isspace(char c);
void		lstadd_back_token(t_token	**head, t_token	*new);
bool		quote_syntax_and_tokenization(t_data	*data);
void		variable_detection(t_token	**node_adress);
bool		forbidden_succes_ops(t_token	**node_adress);
bool		check_variables(t_data	*data);
int			quote_status(char c, int Q_status);
t_token		*lstnew_token(char	*token, bool quoted);

t_command	*create_new_command(t_token **h_node, char **args, bool pipped);
t_file_o	*new_fd(t_data *data, char *filename, int type);

/* COMMANDS */
char		**fill_args(t_token	**node_address, int size);
bool		create_command_list(t_data	*data);
void		lstadd_back_command(t_command	**head, t_command	*new);
char		**split_path_var(t_data	*data);
void		ft_block(t_token **tmp, t_token	**node, bool *flag,	int	*i);
t_token		*find_command(t_token	**h_node);
void		append_list_command(t_data	*data, t_token **tmp, int i, bool pipe);
int			num_cmd(t_token	**head);

/* DEBUGGING */
// void	print_paths(char	**splits);
// void	print_tokens(t_data	*data);
// void	print_command(t_command	**head);

/* PATHS */
char		**split_path_var(t_data	*data);
void		retrieve_path(t_command	**address_node, char	**splitted_paths);
void		assign_path_to_command(t_data	*data);
bool		check_command(char	*command);
char		*join_dash(char *path, char *command);

/* ERROR MSG */
void		error_msg(char	*string, char	*attribute);
char		*symbol_from_int(int a);

/* new tokenization */
int			type_of_token(char *s);
bool		ft_isspace(char c);
int			len_token_no_quotes(char *s);
int			quotes_to_status(char q);
int			quote_status(char c, int Q_status);
int			len_token_quotes(char *s);
char		*token_fill(char *input, int *i, bool *quoted);
bool		contains_quote(char	*s);
t_token		**create_token_list(t_data	*data);
int			quotes_to_status(char q);
void		append_token(t_token **head, char **tmp, bool *quoted);

/* VARIABLE EXPANSION */
void		variable_expand(t_data *data);
bool		expande(char *string, int Q_status, int i);
int			num_of_variables(char *string);
char		**variable_grid(t_data *data, char *string);
bool		there_is_a_var(char *string);
char		*cut_var(char *s);
int			len_till_var_end(char *s);
char		*plug_variables(char *token_content, char **grid);
char		*ft_join_char(char *s, char c);
char		*ft_strdup_one_char(char	*s);
void		cases(t_data	*data, char	**grid, int	*i, char	*string);
bool		ft_is_meta(char c);

/*LEAKS*/
void		delete_char_double(char	**grid);

/* SPECIAL CHARS */
bool		begin_special_var(char c);
char		*special_var_cut(char *s);
bool		ft_is_meta(char c);
void		meta_variables(t_data *data, char **var_left, char **grid, int *i);
char		*ft_strjoin_free(char const *s1, char const *s2);

/* RM QUOTES */
void		remove_quotes(t_data *data);
void		*ft_realloc(void *ptr, size_t size);
char		*rm_from_string(char	*string);
void		rm_node(t_token	**head, t_token	**h_node, void (*func)(t_token **));
bool		special_char_expand(int c);

/* SIGNAL HANDLING */
void		signal_handle_interactive(void);
void		new_prompt(int signum);
void		ft_exit(void);
void		hotkeys_echo(void);

/* ENV LINKED LIST */
int			len_env_value(char *str);
int			len_env_var(char *str);
char		*env_parts(char	*env_line, int var_part);
char		*ft_strndup(char	*s, int n);
void		lstadd_back_var(t_env	**head, t_env	*new);
t_env		*lstnew_var(char	*var, char	*value);
void		print_env(t_data	*data);
char		*get_env_var_value(char	*var, t_env	**head);
bool		init_env(t_data	*data, char	**env);
void		del_env_content(t_env	**h_node);
void		delete_var_env(t_data	*data, char	*var, void (*func)(t_env **));
void		env_to_array(t_data	*data);
int			env_len(t_env	**head);
int			ft_max(int a, int b);

/* REDIRECTION */

int			open_file_parameters(t_data *data, char *filename, int type);
char		*filename_plus(char *filename);
int			num_io_files(t_token **head);
void		list_file_io(t_data *data, t_token **token, t_command **command_n);
void		add_file(t_file_o **head, t_file_o *new_file);
int			last_file_io(t_file_o **head, int type);
bool		check_io_redirect(t_token **head);
bool		is_redirect(int type);
bool		ambiguous_redirect(t_data *data,
				t_token **head_token, t_env **head);

/* HEREDOC */
int			open_heredoc(t_data	*data, char	*delimiter);
char		*create_name_hd(void);
void		check_line(t_data *data, char	**line_ad);
void		heredoc_filling(t_data *data, char	*delim, int *fd, char	*name);
char		*plug_variables_hd(char	*t_content, char	**grid);

/* LEAKS MANAGEMENT */

void		free_tokens(t_token	**head);
void		delete_token_content(t_token	**h_node);
void		free_commands(t_command	**head);
void		free_files(t_file_o	**files);
bool		all_space(char	*s);
char		**variable_grid_hd(t_data *data, char *string);
bool		expand_hd(char	*string, int Q_status, int i);
int			num_token(char	*input);

//**************************************
// EXECUTION
void		execution(t_data *data, t_command **head);

//*************
//BUILTINS
int			exe_uwu(t_data *data, int is_child);

//ECHO
int			m_echo(t_data *data);

//PWD
int			m_pwd(t_data *data);

//ENV
int			m_env(t_data *data);

//EXIT
int			m_exit(t_data *data);

//CD
int			m_cd(t_data *data);

//CD_UTILS
void		u_switch(t_data *data, char *dir, t_garbage *garb, t_env *temp);
int			u_env_switch(t_data *data, char *directory);
int			u_check_pwd(t_data *data);
int			u_cd_default(t_data *data);

//EXPORT
int			m_export(t_data *data);
void		u_declare_print(t_data *data);
char		**copy_env(t_data *data);
void		free_arr(char **arr);

//EXPORT_3
void		add_env(t_data *data, char *new);
int			u_invalid(char *type, char *new);
void		u_new_env(t_env **env, char *new);
void		u_replace_env(t_env **env, char *new, int *flag);

//UNSET
int			m_unset(t_data *data);

//OTHER
int			m_other(t_data *data);

//EXEC_TOOLS
//UTILS
int			ft_strcmp(const char *s1, const char *s2);

void		free_arr(char **arr);
int			u_invalid(char *type, char *new);
int			err_export(char *type, char *new);
char		**split_equal(char *new);
void		u_cmd_check(t_command *head);
int			m_choice(char *choice);
void		free_stuff(t_data	*data);
void		u_redir_builtins(t_data *data);
void		pipe_n_redir(t_data *data);

//CHILD_UTILS
int			u_wait(t_data *data, int is_mpipe);
void		u_cleanup(t_data *data);
void		u_close_curr_cmd(t_data *data);
void		u_redir(t_data *data, int *f_in, int *f_out);

//ERR_MSGS
void		error_message(char *err);
int			err_export(char *type, char *new);
int			u_errexit(char *err, char *type, int num);

//ENDOF EXEC
//*************

#endif
#include "../../minishell.h"

/* Validate variable name - 
	must start with letter/underscore, contain only alphanumeric/underscore
*/
static int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_export_assignment(t_env **env, const char *arg)
{
	char	*key;
	char	*value;

	key = extract_var_name(arg);
	value = extract_var_value(arg);
	if (!is_valid_identifier(key))
	{
		printf("minishell: export: `%s': not a valid identifier\n", key);
		free(key);
		free(value);
		return (1);
	}
	
	// FIXED: Passa direttamente env senza creare puntatore locale
	export_with_assignment(key, value, env);
	free(key);
	free(value);
	return (0);
}

int	handle_export_simple(t_env **env, const char *arg)
{
	if (!is_valid_identifier(arg))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	
	// FIXED: Passa direttamente env senza creare puntatore locale
	export_existing_var(arg, env);
	return (0);
}

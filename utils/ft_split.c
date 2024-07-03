#include "../minishell.h"

size_t	ft_word_count(char const *s, char c)
{
	size_t	i;
	size_t	count;
	int		in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c && in_word == 1)
			in_word = 0;
		i++;
	}
	return (count);
}

static void	*free_all(char **all_words, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(all_words[i]);
		i++;
	}
	free(all_words);
	return (NULL);
}

static char	*allocate_word(char const *s, size_t start, size_t end, int mode)
{
	char	*word;

	if(mode == MANUAL)
	{
		word = (char *)malloc(sizeof(char) * (end - start + 1));
		if (!word)
			return (NULL);
	}
	else
		word = (char *)smart_malloc(sizeof(char) * (end - start + 1));
	ft_strlcpy(word, s + start, end - start + 1);
	return (word);
}

static char	**split_into_words(char const *s, char c, size_t word_count, int mode)
{
	size_t	i;
	size_t	j;
	size_t	start;
	char	**all_words;

	if (mode == MANUAL)
	{
		all_words = malloc(sizeof(char *) * (word_count + 1));
		if (!all_words)
			return (NULL);
	}
	else
		all_words = (char **)smart_malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	j = 0;
	while (j < word_count)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		all_words[j] = allocate_word(s, start, i, mode);
		if (!all_words[j] && mode == MANUAL)
			return (free_all(all_words, j));
		j++;
	}
	all_words[j] = NULL;
	return (all_words);
}

char	**ft_split(char const *s, char c, int mode)
{
	size_t	word_count;
	char	**empty;

	if (!s || !*s)
		return (NULL);
	word_count = ft_word_count(s, c);
	return (split_into_words(s, c, word_count, mode));
}
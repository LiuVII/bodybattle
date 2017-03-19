/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:19:53 by mfilipch          #+#    #+#             */
/*   Updated: 2017/03/18 20:19:54 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h" 

/*
** Free memory allocated within vm structure (if any)
*/
void	free_mem(vm_t *vm)
{
	return ;
}

void	display_usage()
{
	ft_putstr("usage: ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...\n");
	return ;
}

/*
** Calls free function for vm, prints errors messages if any and exits
** Codes:
** 0 - no errors occured
** 1 - limit of players exceeded
** 2 - unsupported flag
** 3 - invalid file
** 4 - champ number already exists
** 5 -
** 6 -
** 7 -
** 8 -
** 9 -
*/
void	free_and_exit(vm_t *vm, int code)
{
	if (vm)
		free_mem(vm);
	if (code == 0)
		exit(0);
	ft_putstr_fd(2, "Error message\n");
	exit(1);
}

/*
** Load champion
*/
void load_champ(vm_t *vm, champ_file, champ_ind)
{
	int	flen;
	int	i;
	int	next_num;

	i = 0;
	next_num = 0;
	while (vm->champs[i])
	{
		if (champ_ind == vm->champs[i]->ind)
			free_and_exit(4);
		if (vm->champs[i]->ind > next_num)
			next_num = vm->champs[i]->ind + 1;
	}
	flen = ft_strlen(champ_file);
	if (flen < 5 || ft_strcmp(&(champ_file[flen-4]), ".cor") != 0
		|| (fd = open(champ_file, R_DONLY)) <= 0)
		free_and_exit(3);
	
	// Allocate memory for champ and increase counter
	vm->champs[vm->champ_num] = (champ_t*)malloc(sizeof(champ_t));
	// Set defaults
	vm->champs[vm->champ_num]->carry = 0;
	vm->champs[vm->champ_num]->live = 0;
	// Set initial PC
	vm->champs[vm->champ_num]->pc = 0;
	if (champ_ind == 0)
		vm->champs[vm->champ_num]->ind = next_num; 
	else
		vm->champs[vm->champ_num]->ind = champ_ind;
	vm->champ_num++;
	
	// ! Set header and load instructons
}

/*
** Parse, check flags and call load champions
*/
void	parse_flags(vm_t *vm, int ac, char const **av)
{
	int	i;
	int	num;

	// ! Simple parser should be replaced with solid one
	i = 0;
	while (av[i])
	{
		num = 0;
		if (av[i][0] == '-')
		{
			if (ft_strcmp(av[i], "-dump") == 0 && i + 1 < ac)
				vm->cycles_to_dump = ft_atoi(av[i + 1]);
			else if (ft_strcmp(av[i], "-n") == 0 && i + 2 < ac)
			{
				num = ft_atoi(av[i + 1]);
				i++;
			}
			else
				free_and_exit(vm, 2);
			i++;
		}
		if (vm->champ_num + 1 > MAX_PLAYERS)
				free_and_exit(vm, 1);
		load_champ(vm, av[i], num);
		i++;
	} 
}

void	init_vm(vm_t *vm)
{
	vm->cycles_to_die = CYCLES_TO_DIE;
	vm->cycles_to_dump = 0;
	vm->cycle = 0;
	vm->champ_num = 0;
	vm->nbr_live_calls = 0;
	bzero(vm->memory, MEM_SIZE);
	bzero(vm->champs, MAX_PLAYERS * sizeof(champ_t*));
}

void	run_vm(vm_t *vm)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (1)
	{
		// Load incturctions into queue
		
		// Execute instructions
		
		// Check conditions
		if (vm->cycle == CYCLES_TO_DIE)
			if (check_alive(vm) == 0)
				break ;
		if (vm->nbr_live_calls == NBR_LIVE || vm->cycle == MAX_CHECKS)
		{
			vm->cycles_to_die -= CYCLE_DELTA;
			vm->cycle = 0;
		}
		else
			vm->cycle++;
	}
	game_over(vm);
}

int	main(int argc, char const **argv)
{
	vm_t	vm;

	if (argc < 3)
	{
		display_usage();
		return (0);
	}
	init_vm(&vm);
	parse_flags(&vm, argc-1, &(argv[1]));
	run_vm(&vm);
	return (0);
}

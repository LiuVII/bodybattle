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
void	free_mem(t_vm *vm)
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
void	free_and_exit(t_vm *vm, int code)
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
void load_champ(t_vm *vm, champ_file, champ_ind)
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
	
	// ! Set header and load instructions
}

/*
** Parse, check flags and call load champions
*/
void	parse_flags(t_vm *vm, int ac, char const **av)
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

void	init_vm(t_vm *vm)
{
	vm->cycles_to_die = CYCLES_TO_DIE;
	vm->cycles_to_dump = -1;
	vm->cycle = 0;
	vm->champ_num = 0;
	vm->nbr_live_calls = 0;
	ft_bzero(vm->memory, MEM_SIZE);
	ft_bzero(vm->champs, MAX_PLAYERS * sizeof(champ_t*));
}

int		get_arg(char *mem, int size, int *pos)
{
	int i;
	int	ans;

	i = 0
	ans = 0
	while (i < size)
	{
		ans = ans << 8;
		ans = mem[pos];
		*pos = (*pos + 1) % MEM_SIZE; 
		i++;
	}
	return (ans);
}

void	proc_init(t_proc *proc)
{
	proc = (t_proc*)malloc(sizeof(t_proc));
	ft_bzero(proc, sizeof(t_proc));
}



/*
** Loading instructions with current params for subsequent execution.
** ! Currently loaded form champ memory but should be from VM_memory,
** or champ memory should change
*/

void	load_instr(t_vm *vm, t_champ *champ, t_proc *proc, int ind)
{
	int	pos;
	int arg_byte;
	int	i;

	pos = champ->pc;
	// If corrupted instruction dunno what to do with it
	if (champ->regs[pos] > 16)
	{
		champ->pc = (pos + 1) % MEM_SIZE;
		return ;
	}
	proc_init(proc);
	proc->champ_id = vm->champ_num - ind - 1;
	proc->proc_id = champ->regs[pos] - 1;
	proc->exec_cycle = g_op_tab[proc->proc_id].cycles;
	pos = (pos + 1) % MEM_SIZE;
	// if there's no encoding byte (live has a special condition of 4 bytes)
	if (proc->proc_id == 0)
		proc->args[0] = get_arg(champ->regs, 4, &pos);
	else if (g_op_tab[proc->proc_id].arg_code_byte == 0)
		proc->args[0] = get_arg(champ->regs, T_DIR, &pos);
	else
	{
		// decipher encoding byte and record params
		arg_byte = champ->regs[pos];
		pos = (pos + 1) % MEM_SIZE;
		proc->arg_types[0] = arg_byte >> 6;
		proc->arg_types[1] = (arg_byte >> 4) % 4;
		proc->arg_types[2] = (arg_byte >> 2) % 4;
		proc->arg_types[3] = arg_byte % 4;
		i = 0;
		while (i < 4)
		{
			if (proc->arg_types[i] == REG_CODE)
				proc->args[i] = get_arg(champ->regs, T_REG, &pos);
			else if (proc->arg_types[i] == DIR_CODE)
				proc->args[i] = get_arg(champ->regs, T_DIR, &pos);
			else if (proc->arg_types[i] == IND_CODE)
				proc->args[i] = get_arg(champ->regs, T_DIR, &pos);
			i++;
		}
	}
	champ->pc = pos;
}

void	exec_instr(t_vm *vm, t_proc *proc, int i)
{
	int res;

	res = g_op_tab[proc->proc_id]->op(vm, proc);
	free(vm->procs[i]);
	vm->procs[i] = 0;
	if (res == 0)
		vm->champs[vm->champ_num - i - 1]->carry = 1;
	else
		vm->champs[vm->champ_num - i - 1]->carry = 0;
}

void	check_alive(t_vm *vm)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (i < vm->champ_num)
	{
		// Check if alive and set dead otherwise
		if (vm->champs[i]->live == 0)
		{
			vm->champs[i]->live = -1;
			// ! Remove from arrays possibly (then no need to check for live)
			// ft_remove_int(vm->champs, i);
			// ft_remove_int(vm->procs, vm->champ_num - i - 1);
			// vm->champ_num--;
			// i--;
		}
		else
		{
			vm->champs[i]->live = 0;
			counter++;
		}
		i++;
	}
	return counter;
}

void	game_over(t_vm *vm)
{
	if (vm->last_alive > 0)
		ft_printf("Player %d (%s) won\n", vm->champs[last_alive]->ind,
			vm->champs[last_alive]->info->prog_name);
	else
		ft_putstr("The many men, so beautiful!\nAnd they all dead did lie.\n");
	free_and_exit(0);
}

void	mem_dump(t_vm *vm)
{
	int 			i;
	char			basestr = "0123456789ABCDEF";
	unsigned char	c;

	i = 0;
	while ((vm->memory)[i])
	{
		c = (unsigned char)(vm->memory)[i];
		write(1, &basestr[c / 16], 1);
		write(1, &basestr[c % 16], 1);
		i++;
		if (i % 32 == 0)
			ft_putstr("\n");
		else
			ft_putstr(" ");
	}
	free_and_exit(0);
}


void	run_vm(t_vm *vm)
{
	int	i;
	int	counter;

	counter = 0;
	while (1)
	{
		// Load incturctions into process queue
		i = 0;
		while (i < vm->champ_num)
		{
			if (vm->procs[i] == 0
				&& vm->champs[vm->champ_num - i - 1]->live >= 0)
				load_instr(vm, vm->champs[vm->champ_num - i - 1], vm->procs[i], i);
			i++;
		}
		// Execute instructions
		i = 0;
		while (++i < vm->champ_num)
		{
			if (vm->procs[i] != 0
				&& vm->champs[vm->champ_num - i - 1]->live >= 0)
			{
				vm->procs[i]->exec_cycle -= 1;
				if (vm->procs[i]->exec_cycle == 0)
					exec_instr(vm, vm->procs[i], i);
			}
			i++;
		}
		// Check conditions
		(vm->cycles_to_dump >= 0) ? vm->cycles_to_dump-- : 0;
		if (vm->cycles_to_dump == 0)
			mem_dump(vm);
		if (vm->cycle == CYCLES_TO_DIE)
		{
			vm->checks = (vm->checks + 1) % MAX_CHECKS;
			if (check_alive(vm) == 0)
				break ;
		}
		if (vm->nbr_live_calls == NBR_LIVE || (vm->checks == 0 && vm->cycle != 0))
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
	t_vm	vm;

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

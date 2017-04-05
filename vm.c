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

const t_op    g_op_tab[17] =
{
	{&op_live, "live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{&op_ld, "ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{&op_st, "st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{&op_add, "add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{&op_sub, "sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{&op_and, "and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{&op_or, "or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{&op_xor, "xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{&op_zjump, "zjump", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{&op_ldi, "ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{&op_sti, "sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{&op_fork, "fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},
	{&op_lld, "lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},
	{&op_lldi, "lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{&op_lfork, "lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{&op_aff, "aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
	{0, 0, 0, {0}, 0, 0, 0, 0, 0}
};

/*
** Free memory allocated within vm structure (if any)
*/
void	free_mem(t_vm *vm)
{
	free(vm);
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
** 5 - invalid magic number
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
	ft_putstr_fd("Error message\n", 2);
	exit(1);
}

void	read_cor(t_vm *vm, int fd, int size)
{
	char *buf;
	int	res;

	buf = (char*)malloc(size + 1);
	res = read(fd, buf, size);
	buf[size] = 0;
	if (size == 4 && (res < 4 || ft_atoi(buf) != COREWAR_EXEC_MAGIC))
		free_and_exit(vm, 5);
	else if (size == PROG_NAME_LENGTH + 4)
	{
		ft_strncpy(vm->champs[vm->champ_num]->info->prog_name, buf, PROG_NAME_LENGTH);
		vm->champs[vm->champ_num]->info->prog_name[PROG_NAME_LENGTH] = 0;
		// Read program size
		read(fd, buf, 4);
		// ! Don't know what to do with it
		// buf[4] = 0;
	}
	else if (size == COMMENT_LENGTH)
	{
		ft_strncpy(vm->champs[vm->champ_num]->info->comment, buf, COMMENT_LENGTH);
		vm->champs[vm->champ_num]->info->comment[COMMENT_LENGTH] = 0;
	}
	ft_bzero(buf, size);
	free(buf);
}

/*
** Save champion's instructions into VM memory
*/
void save_instr(t_vm *vm, int fd, int pos)
{
	char buf[BUFF_SIZE];
	int	res;

	while ((res = read(fd, buf, BUFF_SIZE)) > 0)
	{
		ft_memcpy(&(vm->memory[pos]), buf, res);
		pos += res;
	}
}

/*
** Save champion
*/
void save_champ(t_vm *vm, const char *champ_file, int champ_ind)
{
	int	res;
	int	i;
	int	fd;
	int	next_num;

	i = 0;
	fd = -1;
	next_num = 0;
	while (vm->champs[i])
	{
		if (champ_ind == vm->champs[i]->ind)
			free_and_exit(vm, 4);
		if (vm->champs[i]->ind > next_num)
			next_num = vm->champs[i]->ind + 1;
	}
	res = ft_strlen(champ_file);
	if (res < 5 || ft_strcmp(&(champ_file[res-4]), ".cor") != 0
		|| (fd = open(champ_file, O_RDONLY)) <= 0)
		free_and_exit(vm, 3);
	
	// Check magic
	read_cor(vm, fd, 4);
	// Allocate memory for champ and increase counter
	vm->champs[vm->champ_num] = (t_champ*)malloc(sizeof(t_champ));
	// Set name and save size (+4 because of padding)
	read_cor(vm, fd, PROG_NAME_LENGTH + 4);
	// Set comment
	read_cor(vm, fd, COMMENT_LENGTH);
	// Set defaults
	vm->champs[vm->champ_num]->carry = 0;
	vm->champs[vm->champ_num]->live = 0;
	// Set initial PC
	// ! Should be randomly given by VM
	vm->champs[vm->champ_num]->pc = 0;
	vm->champs[vm->champ_num]->ind = (champ_ind == 0) ? next_num : champ_ind;
	vm->champ_num++;
	// Save instructions
	save_instr(vm, fd, vm->champs[vm->champ_num]->pc);
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
				vm->cycle_to_dump = ft_atoi(av[i + 1]);
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
		save_champ(vm, av[i], num);
		i++;
	} 
}

void	init_vm(t_vm *vm)
{
	vm->cycle_to_die = CYCLE_TO_DIE;
	vm->cycle_to_dump = -1;
	vm->cycle = 0;
	vm->champ_num = 0;
	vm->nbr_live_calls = 0;
	ft_bzero(vm->memory, MEM_SIZE);
	ft_bzero(vm->champs, MAX_PLAYERS * sizeof(t_champ*));
}

int		get_arg(char *mem, int size, int *pos)
{
	int i;
	int	ans;

	i = 0;
	ans = 0;
	while (i < size)
	{
		ans = ans << 8;
		ans = mem[*pos];
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
	if (vm->memory[pos] > 16)
	{
		champ->pc = (pos + 1) % MEM_SIZE;
		return ;
	}
	proc_init(proc);
	proc->champ_id = vm->champ_num - ind - 1;
	proc->proc_id = vm->memory[pos] - 1;
	proc->exec_cycle = g_op_tab[proc->proc_id].cycles;
	pos = (pos + 1) % MEM_SIZE;
	// if there's no encoding byte (live has a special condition of 4 bytes)
	if (proc->proc_id == 0)
		proc->args[0] = get_arg(vm->memory, 4, &pos);
	else if (g_op_tab[proc->proc_id].arg_code_byte == 0)
		proc->args[0] = get_arg(vm->memory, T_DIR, &pos);
	else
	{
		// decipher encoding byte and record params
		arg_byte = vm->memory[pos];
		pos = (pos + 1) % MEM_SIZE;
		proc->arg_types[0] = arg_byte >> 6;
		proc->arg_types[1] = (arg_byte >> 4) % 4;
		proc->arg_types[2] = (arg_byte >> 2) % 4;
		proc->arg_types[3] = arg_byte % 4;
		i = 0;
		while (i < 4)
		{
			if (proc->arg_types[i] == REG_CODE)
				proc->args[i] = get_arg(vm->memory, T_REG, &pos);
			else if (proc->arg_types[i] == DIR_CODE)
				proc->args[i] = get_arg(vm->memory, T_DIR, &pos);
			else if (proc->arg_types[i] == IND_CODE)
				proc->args[i] = get_arg(vm->memory, T_DIR, &pos);
			i++;
		}
	}
	champ->pc = pos;
}

void	exec_instr(t_vm *vm, t_proc *proc, int i)
{
	int res;

	res = g_op_tab[proc->proc_id].op(vm, proc);
	free(vm->procs[i]);
	vm->procs[i] = 0;
	// If operation was successful set carry to 1, otherwise to zero
	if (res == 0)
		vm->champs[vm->champ_num - i - 1]->carry = 1;
	else
		vm->champs[vm->champ_num - i - 1]->carry = 0;
}

int		check_alive(t_vm *vm)
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
		ft_printf("Player %d (%s) won\n", vm->champs[vm->last_alive]->ind,
			vm->champs[vm->last_alive]->info->prog_name);
	else
		ft_putstr("The many men, so beautiful!\nAnd they all dead did lie.\n");
	free_and_exit(vm, 0);
}

void	mem_dump(t_vm *vm)
{
	int 			i;
	char			basestr[16] = "0123456789ABCDEF";
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
	free_and_exit(vm, 0);
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
		(vm->cycle_to_dump >= 0) ? vm->cycle_to_dump-- : 0;
		if (vm->cycle_to_dump == 0)
			mem_dump(vm);
		if (vm->cycle == CYCLE_TO_DIE)
		{
			vm->checks = (vm->checks + 1) % MAX_CHECKS;
			if (check_alive(vm) == 0)
				break ;
		}
		if (vm->nbr_live_calls == NBR_LIVE || (vm->checks == 0 && vm->cycle != 0))
		{
			vm->cycle_to_die -= CYCLE_DELTA;
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

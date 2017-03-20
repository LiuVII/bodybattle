/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instructions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/19 01:39:55 by mfilipch          #+#    #+#             */
/*   Updated: 2017/03/19 01:39:56 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** live: The instruction that allows a process to stay alive.
** Record that the player whose number is the argument is indeed alive. 
** No argument’s coding byte, opcode 0x01. Only argument is on 4 bytes.
** {&op_live, "live", 1, {T_DIR}, 1, 10, "alive", 0, 0}
*/
int				op_live(t_vm *vm, t_proc *proc)
{
	int	num;
	int	i;

	num = (proc->args)[0] % 256;
	i = 0;
	while (i < vm->champ_num)
	{
		if (vm->champs[i]->ind == num)
		{
			ft_printf("A process shows that player %d (%s) is alive\n",
				num, vm->champs[i]->info->prog_name);
			vm->champs[i]->live += 1;
			vm->nbr_live_calls += 1;
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** ld: take an argument and a registry. Load the value of the first argument
** in the registry. Its opcode is 2 and it will change the carry.
** {&op_ld, "ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0}
*/
int				op_ld(t_vm *vm, t_proc *proc)
{
	if (!proc->arg_types[0] || proc->arg_types[1] != REG_CODE)
		return (1);
	address = (vm->champs[proc->champ_id]->r1
	+ (proc->arg[1] - 1) * REG_SIZE) % MEM_SIZE;
	if (proc->arg_types[0] == REG_CODE)
		ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	else if (proc->arg_types[0] == DIR_CODE)
		ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	else if (proc->arg_types[0] == IND_CODE)
		ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	else
		return (1);
	vm->champs[proc->champ_id]->carry = 1 - vm->champs[proc->champ_id]->carry;
	return (0);
}

/*
** st: take a registry and a registry or an indirect and store the value
** of the registry toward a second argument. Its opcode is 3.
** For example, st r1, 42 store the value of r1 at (PC + (42 % IDX_MOD))
** {&op_st, "st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0}
*/
int				op_st(t_vm *vm, t_proc *proc)
{
	if (proc->arg_types[0] != REG_CODE ||
		(proc->arg_types[1] != REG_CODE && proc->arg_types[1] != IND_CODE))
		return (1);
	if (proc->arg_types[1] = REG_CODE)
	{
		address = (vm->champs[proc->champ_id]->r1
		+ (proc->arg[1] - 1) * REG_SIZE) % MEM_SIZE;
		ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	}
	else
	{
		address = (proc->pc + proc->arg[1] % IDX_MOD) % MEM_SIZE;
		ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	}
	return (0);
}

/*
** {&op_add, "add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0}
*/
int				op_add(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_sub, "sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0}
*/
int				op_sub(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** and: Apply an & (bit-to-bit AND) over the first two arguments and
** store the result in a registry, which is the third argument.
** Opcode 0x06. Modifies the carry.
** {&op_and, "and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},
** 6, 6, "et (and  r1, r2, r3   r1&r2 -> r3", 1, 0}
*/
int				op_and(t_vm *vm, t_proc *proc)
{
	int value;

	if (!proc->arg_types[0] || !proc->arg_types[1]
		|| proc->arg_types[2] != REG_CODE)
		return (1);
	value = proc->arg[0] & proc->arg[1];
	address = (vm->champs[proc->champ_id]->r1
		+ (proc->arg[2] - 1) * REG_SIZE) % MEM_SIZE;
	ft_strncpy(&(vm->memory[address]), value, REG_SIZE);
	return (0);
}

/*
** {&op_or, "or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
** 7, 6, "ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0}
*/
int				op_or(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_xor, "xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
** 8, 6, "ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0}
*/
int				op_xor(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** zjmp: no argument’s coding byte, opcode is 9.
** It will take an index and jump to this address if the carry is 1
** {&op_zjmp, "zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1}
*/
int				op_zjump(t_vm *vm, t_proc *proc)
{
	if (vm->champs[proc->champ_id]->carry == 0)
		return (1); 
	vm->champs[proc->champ_id]->pc = (proc->pc +
		proc->arg[0] % IDX_MOD) % MEM_SIZE;
	return (0);
}

/*
** {&op_ldi, "ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
** 10, 25, "load index", 1, 1}
*/
int				op_ldi(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** Opcode 11. Take a registry, and two indexes (potentially registries)
** add the two indexes, and store the result of first param in that address.
** {&op_sti, "sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},
** 11, 25, "store index", 1, 1}
*/
int				op_sti(t_vm *vm, t_proc *proc)
{
	int	value;

	if (proc->arg_types[0] != REG_CODE ||
		!proc->arg_types[1] || !proc->arg_types[2])
		return (1);
	address = (proc->pc + (proc->arg[1] + proc->arg[2]) % IDX_MOD) % MEM_SIZE;
	ft_strncpy(&(vm->memory[address]), proc->arg[0], REG_SIZE);
	return (0);
}

/*
** {&op_fork, "fork", 1, {T_DIR}, 12, 800, "fork", 0, 1}
*/
int				op_fork(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_lld, "lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0}
*/
int				op_lld(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_lldi, "lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
** 14, 50, "long load index", 1, 1}
*/
int				op_lldi(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_lfork, "lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1}
*/
int				op_lfork(t_vm *vm, t_proc *proc)
{
	return (0);
}

/*
** {&op_aff, "aff", 1, {T_REG}, 16, 2, "aff", 1, 0}
*/
int				op_aff(t_vm *vm, t_proc *proc)
{
	return (0);
}

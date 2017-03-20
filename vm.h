/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:47:40 by mfilipch          #+#    #+#             */
/*   Updated: 2017/03/18 20:47:41 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
#define VM_H
# include "op.h"
# include "libft.h"

typedef struct	s_champ
{
	int			r1;
	int			ind;
	header_t*	info;
	int			pc;
	int			carry;
	char		regs[REG_NUMBER][REG_SIZE];
	int			live;


}				t_champ;

typedef struct	s_proc
{
	int			pc;
	int			champ_id;
	int			exec_cycle;
	int			proc_id;
	int			args[MAX_ARGS_NUMBER];
	int			arg_types[MAX_ARGS_NUMBER];
	int			arg_count;

}				t_proc;

typedef struct	s_vm
{
	char		memory[MEM_SIZE];
	champ_t*	champs[MAX_PLAYERS];
	int			champ_num;
	int			cycle;
	int			cycles_to_die;
	int			cycles_to_dump;
	int			nbr_live_calls;
	proc_t*		procs[MAX_PLAYERS];
	int			last_alive;
	int			checks;

}				t_vm;

typedef struct	s_op
{
	int			(*op)(t_vm *, t_proc *);
	char*		name;
	int			max_args;
	int*		params;
	int			op_code;
	int			cycles;
	char*		comment;
	int			arg_code_byte;
	int			smth;
}				t_op;

int				op_add(t_vm *vm, t_proc *proc);
int				op_aff(t_vm *vm, t_proc *proc);
int				op_and(t_vm *vm, t_proc *proc);
int				op_fork(t_vm *vm, t_proc *proc);
int				op_ld(t_vm *vm, t_proc *proc);
int				op_ldi(t_vm *vm, t_proc *proc);
int				op_lfork(t_vm *vm, t_proc *proc);
int				op_live(t_vm *vm, t_proc *proc);
int				op_lld(t_vm *vm, t_proc *proc);
int				op_lldi(t_vm *vm, t_proc *proc);
int				op_or(t_vm *vm, t_proc *proc);
int				op_st(t_vm *vm, t_proc *proc);
int				op_sti(t_vm *vm, t_proc *proc);
int				op_sub(t_vm *vm, t_proc *proc);
int				op_xor(t_vm *vm, t_proc *proc);
int				op_zjump(t_vm *vm, t_proc *proc);

// const 	t_op	g_op_tab[] =
// {
// 	{1, &op_live},
// 	{2, &op_ld},
// 	{3, &op_st},
// 	{4, &op_add},
// 	{5, &op_sub},
// 	{6, &op_and},
// 	{7, &op_or},
// 	{8, &op_xor},
// 	{9, &op_zjump},
// 	{10, &op_ldi},
// 	{11, &op_sti},
// 	{12, &op_fork},
// 	{13, &op_lld},
// 	{14, &op_lldi},
// 	{15, &op_lfork},
// 	{16, &op_aff}
// };

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
	{&op_zjmp, "zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
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

#endif

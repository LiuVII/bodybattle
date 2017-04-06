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
# include "ft_printf.h"
# include <fcntl.h>
# include <unistd.h>

typedef struct	s_champ
{
	int			r1;
	int			ind;
	header_t	info;
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
	t_champ*	champs[MAX_PLAYERS];
	int			champ_num;
	int			cycle;
	int			cycle_to_die;
	int			cycle_to_dump;
	int			nbr_live_calls;
	t_proc*		procs[MAX_PLAYERS];
	int			last_alive;
	int			checks;

}				t_vm;

typedef struct	s_op
{
	int			(*op)(t_vm *, t_proc *);
	char*		name;
	int			max_args;
	int			params[3];
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

#endif

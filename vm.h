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

typedef struct	champ_s
{
	int			ind;
	header_t*	info;
	char*		pc;
	int			carry;
	char		regs[REG_NUMBER][REG_SIZE];
	int			live;


}				champ_t;

typedef struct	proc_s
{
	int			champ_id;
	int			exec_cycle;
	int			proc_id;
	int			params[MAX_ARGS_NUMBER];
	int			params_count;

}				proc_t;

typedef struct	vm_s
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

}				vm_t;

#endif

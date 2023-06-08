/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpalusze <cpalusze@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 13:47:36 by cpalusze          #+#    #+#             */
/*   Updated: 2023/06/08 13:47:36 by cpalusze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum {
	CHANNELMOD,
	USERMOD,
	ADD,
	RM,

	O_ADD_OP_USERMOD,
	O_RM_OP_USERMOD,
	O_ADD_OP_CHANNELMOD,
	O_RM_OP_CHANNELMOD,
	L_ADD_CLIENTLIMIT_CHANNELMOD,
	L_RM_CLIENTLIMIT_CHANNELMOD,
	I_ADD_INVITEONLY_CHANNELMOD,
	I_RM_INVITEONLY_CHANNELMOD,
	T_ADD_PROTECTEDTOPIC_CHANNELMOD,
	T_RM_PROTECTEDTOPIC_CHANNELMOD,
	K_ADD_KEY_CHANNELMOD,
	K_RM_KEY_CHANNELMOD,
	N_ADD_CHANNELMODE,
	N_RM_CHANNELMODE,

	UNKNOWN_FLAG,
} FLAGS_MODE;

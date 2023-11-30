/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:01:22 by jyao              #+#    #+#             */
/*   Updated: 2023/11/30 14:10:37 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"CGIhandler.hpp"
#include	"Request.hpp"
#include	"Response.hpp"


int	main(void)
{
	CGIhandler handler;
	std::cout<<handler.executeCGI("s1.sh");
	return (0);
};
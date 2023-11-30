/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:01:22 by jyao              #+#    #+#             */
/*   Updated: 2023/11/30 15:06:24 by jyao             ###   ########.fr       */
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
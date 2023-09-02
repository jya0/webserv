/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 12:27:38 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<string>
# include	<vector>

/**
 * @brief The abstract class inherited by DirectiveBlock and DirectiveSimple
 */
class	ADirective {
	protected:
		std::string	_dveName;	///used as the key for the map of Directive
		std::vector< std::string >	_dveValues;	///values for 
		std::string	_dveType;
	public:
		ADirective(void);
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		std::string	getName(void) const;
		std::vector< std::string >	getValues(void) const;
		std::string	getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
};

#endif
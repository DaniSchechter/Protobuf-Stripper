#ifndef REGEX_HPP

#define REGEX_HPP

#include <regex>
#include <iostream>
#include "TestsConfig.hpp"


class Regex
{
  	static Regex *s_instance;

public:
  std::regex google_api_key;
  std::regex twitter_access_token;
  std::regex facebook_access_token;
  std::regex google_auth_id;
  std::regex picatic_api_key;
  std::regex stripe_standard_api_key;
  std::regex stripe_restricted_api_key;
  std::regex square_access_token;
  std::regex square_auth_secret;
  std::regex paypal_braintree_access_token;
  std::regex amazon_mws_auth_token;
  std::regex twilio_api_key;
  std::regex mailgun_api_key;
  std::regex mailchimp_api_key;
  std::regex amazon_aws_access_key_id;



	Regex()
	{
    google_api_key=std::regex("AIza[0-9A-Za-z[-]_]{35}");
    twitter_access_token=std::regex("[1-9][0-9]+-[0-9a-zA-Z]{40}");
    facebook_access_token=std::regex("EAACEdEose0cBA[0-9A-Za-z]+");
    google_auth_id = std::regex("[0-9]+-[0-9A-Za-z_]1321[.]apps[.]googleusercontent[.]com");
    picatic_api_key=std::regex("sk_live_[0-9a-z]{32}");
    stripe_standard_api_key=std::regex("sk_live_[0-9a-zA-Z]{24}");
    stripe_restricted_api_key=std::regex("rk_live_[0-9a-zA-Z]{24}");
    square_access_token=std::regex("sqOatp-[0-9A-Za-z[-]_]{22}"); 
    square_auth_secret=std::regex("sq0csp-[0-9A-Za-z[-]_]{43}");
    paypal_braintree_access_token=std::regex("access_token[$]production[$][0-9a-z]{16}[$][0-9a-f]{32}"); 
    amazon_mws_auth_token=std::regex("amzn[.]mws[.][0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}");
    twilio_api_key=std::regex("SK[0-9a-fA-F]{32}");
    mailgun_api_key=std::regex("key-[0-9a-zA-Z]{32}");
    mailchimp_api_key=std::regex("[0-9a-f]{32}-us[0-9]{1,2}");
    amazon_aws_access_key_id=std::regex("AKIA[0-9A-Z]{16}");

	}
	static Regex* instance()
	{
		if (!s_instance)
			s_instance = new Regex;
		return s_instance;
	}
};

#endif

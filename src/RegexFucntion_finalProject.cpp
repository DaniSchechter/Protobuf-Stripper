
#include "regexClass.hpp"


Regex *Regex::s_instance = 0;

 bool findTokens(std::string data)
 {

    std::smatch m;
    return (regex_search(data, m, Regex::instance()->google_api_key) || regex_search(data, m, Regex::instance()->twitter_access_token) ||
      regex_search(data, m, Regex::instance()->facebook_access_token) || regex_search(data, m, Regex::instance()->google_auth_id) ||
      regex_search(data, m, Regex::instance()->picatic_api_key) || regex_search(data, m, Regex::instance()->stripe_standard_api_key) ||
      regex_search(data, m, Regex::instance()->stripe_restricted_api_key) || regex_search(data, m, Regex::instance()->square_access_token) ||
      regex_search(data, m, Regex::instance()->square_auth_secret) || regex_search(data, m, Regex::instance()->paypal_braintree_access_token) ||
      regex_search(data, m, Regex::instance()->amazon_mws_auth_token) || regex_search(data, m, Regex::instance()->twilio_api_key) ||
      regex_search(data, m, Regex::instance()->mailgun_api_key) || regex_search(data, m, Regex::instance()->mailchimp_api_key) ||
      regex_search(data, m, Regex::instance()->amazon_aws_access_key_id));
 }

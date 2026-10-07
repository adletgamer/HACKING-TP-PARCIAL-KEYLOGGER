#include <iostream>
#include <curl/curl.h>

int main()
{
    CURL* curl = curl_easy_init();

    if (curl)
    {
        std::cout << "libcurl funciona correctamente." << std::endl;

        std::cout << "Version: "
                  << curl_version()
                  << std::endl;

        curl_easy_cleanup(curl);
    }
    else
    {
        std::cout << "Error al inicializar libcurl." << std::endl;
    }

    return 0;
}
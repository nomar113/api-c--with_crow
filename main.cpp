#include "crow.h"
//#include "crow_all.h"

int main()
{
    crow::SimpleApp app; //define your crow application

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/site")([](){
        auto page = crow::mustache::load_text("fancypage.html");
        return page;
    });

    CROW_ROUTE(app, "/string/<string>")([](std::string name){ // 
        auto page = crow::mustache::load("fancypage.html"); // 
        crow::mustache::context ctx ({{"person", name}}); // 
        return page.render(ctx); //
    });

    CROW_ROUTE(app, "/json")
    ([]{
        crow::json::wvalue x({});
        x["message2"] = "Hello, World.. Again!";
        x["message1"] = "Hello, World!";
        return x;
    });

    CROW_ROUTE(app, "/add_json")
    .methods("POST"_method)
    ([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        int sum = x["a"].i()+x["b"].i();
        std::ostringstream os;
        os << sum;
        return crow::response{os.str()};
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080)
        // .multithreaded() // use 12 multithreads 
        .run();
}

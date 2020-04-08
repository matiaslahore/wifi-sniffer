// ============================
//  Puerto
// ============================
process.env.PORT = process.env.PORT || 3000;


// ============================
//  Environment
// ============================
process.env.NODE_ENV = process.env.NODE_ENV || 'dev';


// ============================
//  Database
// ============================
let urlDB;

if (process.env.NODE_ENV === 'dev') {
    urlDB = 'url_db';
} else {
    //urlDB = process.env.SQL_URI; //Saca la url de la db del entorno (seguridad)
}
process.env.URLDB = urlDB;
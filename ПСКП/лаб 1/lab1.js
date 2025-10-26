const http=require('http');
const port=2000;
const server=http.createServer((req, res)=>{
    res.writeHead(200, {'content-type':'text/HTML; charset=utf-8'});
    res.end('<h1>Hello world</h1>');
});
server.listen(port,()=>{
    console.log(`сервер запущен по адресу http://localhost:${port}`);
});
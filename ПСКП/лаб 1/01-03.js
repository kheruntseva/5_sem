const http = require('http');
const url = require('url');

const PORT = 4000;

const server = http.createServer((req, res) => {

  const parsedUrl = url.parse(req.url, true);
  const pathname = parsedUrl.pathname;
  const queryParams = parsedUrl.query;

  let body = '';
  if (req.method === 'POST') {
    req.on('data', (chunk) => {
      body += chunk.toString();
    });

    req.on('end', () => {
      sendResponse(req, res, pathname, queryParams, body);
    });
  } else {
    sendResponse(req, res, pathname, queryParams, body);
  }
});

function sendResponse(req, res, pathname, queryParams, body) {
  res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });

  const htmlResponse = `
    <!DOCTYPE html>
    <html lang="ru">
    <head>
        <meta charset="UTF-8">
        <title>Детали запроса</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 40px; }
            h1 { color: #333; }
            table { border-collapse: collapse; width: 100%; }
            th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
            th { background-color: #f2f2f2; }
            pre { background-color: #f4f4f4; padding: 10px; border-radius: 5px; }
        </style>
    </head>
    <body>
        <h1>Информация о вашем запросе</h1>
        <table>
            <tr><th>Метод</th><td>${req.method}</td></tr>
            <tr><th>URI (Путь)</th><td>${pathname}</td></tr>
            <tr><th>Query-параметры</th><td>${JSON.stringify(queryParams)}</td></tr>
        </table>
        
        <h2>Заголовки запроса</h2>
        <table>
            ${Object.entries(req.headers).map(([key, value]) => `<tr><th>${key}</th><td>${value}</td></tr>`).join('')}
        </table>
        
        ${req.method === 'POST' ? `
        <h2>Тело POST-запроса</h2>
        <pre>${body}</pre>
        ` : ''}
    </body>
    </html>
  `;
  res.end(htmlResponse);
}

server.listen(PORT, () => {
  console.log(`Сервер запущен на http://localhost:${PORT}`);
});
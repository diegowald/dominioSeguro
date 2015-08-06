<?php
require 'vendor/autoload.php';

//load required files
require './vendor/slim/slim/Slim/Slim.php';
require 'rb.php';

// register Slim auto-loader
\Slim\Slim::registerAutoloader();

// set up database connection
R::setup('mysql:host=192.168.0.58;dbname=hbobroker_db;port=3306', 'hbo_mobile', 'hbo_mobile');
R::freeze(true);

// initialize app
$app = new \Slim\Slim();

// handle get request for /dni
$app->get('/datos', function() use ($app) {
  // query database for all dnis
  $dnis = R::find('datos');

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($dnis));
});

$app->get('/datos/:dni', function($dni) use ($app) {
  // query database for all data with specific dni
  $datos = R::find('datos', ' dni = ? ', [ $dni ]);

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($datos));
})->conditions(array('dni' => '([0-9]{8})'));

$app->post('/register', function() use ($app) {
  try {
    // get and decode JSON request body
    $request = $app->request();
    $post = $request->post();
    $input = $post;

    // store registration form
    $regForm = R::dispense('registracion');
    $regForm->dni = (string)$input['dni'];
    $regForm->celular = (string)$input['celular'];
    $regForm->nombre = (string)$input['nombre'];
    $regForm->fechaSolicitud = (string)$input['fechaSolicitud'];
    $regForm->fechaRegistracion = null;
    $id = R::store($regForm);

    // return JSON-encoded response body
    $app->response()->header('Content-Type', 'application/json');
    echo json_encode(R::exportAll($regForm));

  } catch (Exception $e) {
    $app->response()->status(400);
    $app->response()->header('X-Status-Reason', $e->getMessage());
  }
});

$app->get('/registration_requests', function() use ($app) {
  // query database for all dnis
  $registraciones = R::find('registracion');

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($registraciones));
});

$app->post('/approve/:dni', function($dni) use ($app) {
  // query database for all data with specific dni
  $datos = R::find('datos', ' dni = ? ', [ $dni ]);

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($datos));
})->conditions(array('dni' => '([0-9]{8})'));

$app->get('/stats', function() use ($app) {
  // query database for statistics
  $stats = R::getAll(
"select 1 as id,
(select count(1) from datos) as countDatos,
(select count(1) from registracion) as totalRegistrados,
(select count(1) from registracion where fecha_registracion is null) as regsSolicitados;"
    );

  // convert to beans
  $statistics = R::convertToBeans('statistics', $stats);

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');


  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($statistics));
});

$app->get('/hello/:name', function ($name) use ($app) {
    echo "Hello, " . $name;
});
$app->run();

?>

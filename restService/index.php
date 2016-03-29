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
  $dnis = R::findOne('datos');

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
})->conditions(array('dni' => '([0-9]{7,8})'));

$app->get('/datosMultiDNI/:dnis', function($dnis) use ($app) {
  // query database for all data with specific dni
  $datos = R::find('datos', ' dni in (?) ', [ $dnis ]);

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($datos));
});

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

    // find a record with taht dni
    $dato = R::findOne('datos', ' dni = ? ', [ (string)$input['dni'] ]);
    // return JSON-encoded response body

    $app->response()->header('Content-Type', 'application/json');
    echo json_encode(R::exportAll($dato));

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

$app->post('/approve', function() use ($app) {
  // get and decode JSON request body
  $request = $app->request();
  $post = $request->post();
  $input = $post;

  $id = (int)$input['id'];

  // query database for all data with specific dni
  $datos = R::load('registracion', $id);

  $datos->fecha_registracion = date("Y-m-d H:i:s");

  R::store($datos);

  // send response header for JSON content type
  $app->response()->header('Content-Type', 'application/json');

  // return JSON-encoded response body with query results
  echo json_encode(R::exportAll($datos));
});

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


$app->post('/addrecord', function() use ($app) {
  try {
    // get and decode JSON request body
    $request = $app->request();
    $post = $request->post();
    $input = $post;

    $dato = R::dispense('datos');

    $dato->dni = (string)$input['dni'];
    $dato->dominio = (string)$input['dominio'];
    $dato->asegurado = (string)$input['asegurado'];
    $dato->cobertura = (string)$input['cobertura'];
    $dato->poliza = (string)$input['poliza'];
    $dato->vigencia_desde = (string)$input['vigencia_desde'];
    $dato->vigencia_hasta = (string)$input['vigencia_hasta'];
    $dato->modelo = (string)$input['modelo'];
    $dato->anio = (string)$input['anio'];
    $dato->chasis = (string)$input['chasis'];
    $dato->motor = (string)$input['motor'];
    $dato->mediopago = (string)$input['medioPago'];
    $dato->productor = (string)$input['Productor'];

    R::store($dato);

    // send response header for JSON content type
    $app->response()->header('Content-Type', 'application/json');

    // return JSON-encoded response body with query results
    echo json_encode(R::exportAll($dato));

  } catch (Exception $e) {
    $app->response()->status(400);
    $app->response()->header('X-Status-Reason', $e->getMessage());
  }
});

$app->post('/uploadcsv', function() use ($app) {
  R::begin();

  try {
    if (!isset($_FILES['upload'])) {
      echo "No files uploaded";
      return;
    }

    $files = $_FILES['upload'];
    $cnt = count($files['name']);

    $request = $app->request();
    $post = $request->post();
    $input = $post;

    $columnSeparator = (string)$input['columnSeparator'];
    $stringDelimiter = (string)$input['stringDelimiter'];
    $recordSeparator = (string)$input['recordSeparator'];
    $numLinesToIgnore = (string)$input['numLinesToIgnore'];

    $filename = $files['tmp_name'];

    /*$sql =  
" LOAD DATA LOCAL INFILE '$filename' INTO TABLE datos 
 FIELDS TERMINATED BY '$columnSeparator' OPTIONALY ENCLOSED BY '$stringDelimiter' 
 LINES TERMINATED BY '$recordSeparator' 
 IGNORE $numLinesToIgnore LINES
 ( compania, dni, dominio, asegurado, cobertura,
  poliza, vigencia_desde, vigencia_hasta, modelo, anio, 
  chasis, motor, medioPago, Productor);" ;


    echo $sql;


    $link = mysql_connect('192.168.0.58', 'hbo_mobile', 'hbo_mobile');
    if (!$link) {
        die('Could not connect: ' . mysql_error());
    }
    echo 'Connected successfully';
    
    mysql_select_db('hbobroker_db', $link);

    $result = mysql_query($sql, $link);

    //R::exec($sql);

    if ($result) {
      echo "ok";
    } else {
      echo " error en loaddata ";
      //echo mysql_errno($link) . ": " . mysql_error($link). "\n";
    }

    mysql_close($link);*/

    R::wipe('datos');
    
    $row = 1;
    if (($handle = fopen($filename, "r")) !== FALSE) {
        while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
            if ($row > $numLinesToIgnore) {
              $dato = R::dispense('datos');

              $dato->compania = $data[0];
              $dato->dni = $data[1];
              $dato->dominio = $data[2];
              $dato->asegurado = $data[3];
              $dato->cobertura = $data[4];
              $dato->poliza = $data[5];
              $dato->vigencia_desde = $data[6];
              $dato->vigencia_hasta  = $data[7];
              $dato->marca = $data[8];
              $dato->modelo = $data[9];
              $dato->anio = $data[10];
              $dato->chasis = $data[11];
              $dato->motor  = $data[12];
              $dato->mediopago = $data[13];
              $dato->productor = $data[14];

              R::store($dato);
            }
            $row++;
        }
        fclose($handle);
        R::commit();
    }    
  } catch (Exception $e) {
    R::rollback();
    $app->response()->status(400);
    $app->response()->header('X-Status-Reason', $e->getMessage());
  }
});

$app->get('/hello/:name', function ($name) use ($app) {
    echo "Hello, " . $name;
});
$app->run();

?>

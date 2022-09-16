<?php

namespace App\Http\Controllers;

use App\Models\SolarMeasurementEntry;
use Illuminate\Http\Request;

class MainController extends Controller
{
    public function index() {
        $entries = SolarMeasurementEntry::all()->sortByDesc('timestamp');
        foreach($entries as $entry) {
            echo $entry->watts . ' ';
        }
    }
}

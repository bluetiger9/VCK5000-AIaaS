/*
 * Copyright © 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
#include <cstddef>
#include <glog/logging.h>

#include <iostream>
#include <ostream>
#include <memory>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vitis/ai/lanedetect.hpp>

void output_result(vitis::ai::RoadLineResult &result) {
  std::cout << "{" << std::endl << "  \"results\": [" << std::endl;
  for (auto it = begin(result.lines); it != end(result.lines); ++it)
  {
    auto r = *it;
    std::cout << "    { \"type\": " << r.type << ", \"points\": [" << std::endl;
    for (auto itPoint = begin(r.points_cluster); itPoint != end(r.points_cluster); ++itPoint)
    {
      auto point = *itPoint;
      std::cout << "        { \"x\": " << point.x << ", \"y\": " << point.y << " }";
      if (itPoint != end(r.points_cluster) - 1) {
        std::cout << ",";
      }
      std::cout << std::endl;
    }

    std::cout << "      ]" << std::endl << "    }";
    if (it != end(result.lines) - 1) {
      std::cout << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "  ]" << std::endl << "}" << std::endl;
}

void classify(std::string model_name, std::string image_file_name)
{
  auto model = vitis::ai::RoadLine::create(model_name);
  auto image = cv::imread(image_file_name);
  if (image.empty())
  {
    LOG(FATAL) << "cannot load " << image_file_name << std::endl;
    abort();
  }

  auto result = model->run(image);
  output_result(result);
}

int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);

  if (argc != 3) {
    LOG(FATAL) << "Usage: " << argv[0] << " <model> <image-path>" << std::endl;
    return -1;
  }

  LOG(INFO) << "Vitis AI Lane Detect MicroApp" << std::endl;;
  std::string model = argv[1];
  std::string image_path = argv[2];

#if !defined VITIS_DUMMY_RESULTS
  classify(model, image_path);
  return 0;

#else
  // dummy results mode
  vitis::ai::RoadLineResult result;

  std::vector< cv::Point > points_cluster_1 = { { 164, 377 }, { 165, 376 }, { 166, 375 }, { 167, 373 }, { 168, 372 }, { 169, 371 }, { 170, 369 }, { 171, 368 }, { 172, 367 }, { 173, 365 }, { 174, 364 }, { 175, 363 }, { 176, 362 }, { 177, 360 }, { 178, 359 }, { 179, 358 }, { 180, 356 }, { 181, 355 }, { 182, 354 }, { 183, 352 }, { 184, 351 }, { 185, 350 }, { 186, 348 }, { 187, 347 }, { 188, 346 }, { 189, 345 }, { 190, 343 }, { 191, 342 }, { 192, 341 }, { 193, 339 }, { 194, 338 }, { 195, 337 }, { 196, 335 }, { 197, 334 }, { 198, 333 }, { 199, 332 }, { 200, 330 }, { 201, 329 }, { 202, 328 }, { 203, 326 }, { 204, 325 }, { 205, 324 }, { 206, 322 }, { 207, 321 }, { 208, 320 }, { 209, 319 }, { 210, 317 }, { 211, 316 }, { 212, 315 }, { 213, 313 }, { 214, 312 }, { 215, 311 }, { 216, 309 }, { 217, 308 }, { 218, 307 }, { 219, 305 }, { 220, 304 }, { 221, 303 }, { 222, 302 }, { 223, 300 }, { 224, 299 }, { 225, 298 }, { 226, 296 }, { 227, 295 }, { 228, 294 }, { 229, 292 }, { 230, 291 }, { 231, 290 }, { 232, 289 }, { 233, 287 }, { 234, 286 }, { 235, 285 }, { 236, 283 }, { 237, 282 }, { 238, 281 }, { 239, 279 }, { 240, 278 }, { 241, 277 }, { 242, 276 }, { 243, 274 }, { 244, 273 }, { 245, 272 }, { 246, 270 }, { 247, 269 }, { 248, 268 }, { 249, 266 }, { 250, 265 }, { 251, 264 }, { 252, 262 }, { 253, 261 }, { 254, 260 }, { 255, 259 }, { 256, 257 }, { 257, 256 }, { 258, 255 }, { 259, 253 }, { 260, 252 }, { 261, 251 }, { 262, 249 }, { 263, 248 }, { 264, 247 }, { 265, 246 }, { 266, 244 }, { 267, 243 }, { 268, 242 }, { 269, 240 }, { 270, 239 }, { 271, 238 }, { 272, 236 }, { 273, 235 }, { 274, 234 }, { 275, 233 }, { 276, 231 }, { 277, 230 }, { 278, 229 }, { 279, 227 }, { 280, 226 }, { 281, 225 }, { 282, 223 }, { 283, 222 }, { 284, 221 }, { 285, 219 }, { 286, 218 }, { 287, 217 }, { 288, 216 }, { 289, 214 }, { 290, 213 }, { 291, 212 }, { 292, 210 }, { 293, 209 }, { 294, 208 }, { 295, 206 }, { 296, 205 }, { 297, 204 }, { 298, 203 }, { 299, 201 }, { 300, 200 } };
  result.lines.push_back({ 3, points_cluster_1 });

  std::vector< cv::Point > points_cluster_2 = { { 340, 202 }, { 341, 203 }, { 342, 205 }, { 343, 206 }, { 344, 207 }, { 345, 209 }, { 346, 210 }, { 347, 211 }, { 348, 213 }, { 349, 214 }, { 350, 215 }, { 351, 217 }, { 352, 218 }, { 353, 219 }, { 354, 221 }, { 355, 222 }, { 356, 224 }, { 357, 225 }, { 358, 226 }, { 359, 228 }, { 360, 229 }, { 361, 230 }, { 362, 232 }, { 363, 233 }, { 364, 234 }, { 365, 236 }, { 366, 237 }, { 367, 239 }, { 368, 240 }, { 369, 241 }, { 370, 243 }, { 371, 244 }, { 372, 245 }, { 373, 247 }, { 374, 248 }, { 375, 249 }, { 376, 251 }, { 377, 252 }, { 378, 253 }, { 379, 255 }, { 380, 256 }, { 381, 258 }, { 382, 259 }, { 383, 260 }, { 384, 262 }, { 385, 263 }, { 386, 264 }, { 387, 266 }, { 388, 267 }, { 389, 268 }, { 390, 270 }, { 391, 271 }, { 392, 273 }, { 393, 274 }, { 394, 275 }, { 395, 277 }, { 396, 278 }, { 397, 279 }, { 398, 281 }, { 399, 282 }, { 400, 283 }, { 401, 285 }, { 402, 286 }, { 403, 287 }, { 404, 289 }, { 405, 290 }, { 406, 292 }, { 407, 293 }, { 408, 294 }, { 409, 296 }, { 410, 297 }, { 411, 298 }, { 412, 300 }, { 413, 301 }, { 414, 302 }, { 415, 304 }, { 416, 305 }, { 417, 307 }, { 418, 308 }, { 419, 309 }, { 420, 311 }, { 421, 312 }, { 422, 313 }, { 423, 315 }, { 424, 316 }, { 425, 317 }, { 426, 319 }, { 427, 320 }, { 428, 321 } };
  result.lines.push_back({ 1, points_cluster_2 });

  std::vector< cv::Point > points_cluster_3 = { { 396, 212 }, { 397, 213 }, { 398, 213 }, { 399, 214 }, { 400, 214 }, { 401, 215 }, { 402, 215 }, { 403, 216 }, { 404, 216 }, { 405, 217 }, { 406, 217 }, { 407, 218 }, { 408, 218 }, { 409, 219 }, { 410, 220 }, { 411, 220 }, { 412, 221 }, { 413, 221 }, { 414, 222 }, { 415, 222 }, { 416, 223 }, { 417, 223 }, { 418, 224 }, { 419, 224 }, { 420, 225 }, { 421, 225 }, { 422, 226 }, { 423, 226 }, { 424, 227 }, { 425, 228 }, { 426, 228 }, { 427, 229 }, { 428, 229 }, { 429, 230 }, { 430, 230 }, { 431, 231 }, { 432, 231 }, { 433, 232 }, { 434, 232 }, { 435, 233 }, { 436, 233 }, { 437, 234 }, { 438, 234 }, { 439, 235 }, { 440, 236 }, { 441, 236 }, { 442, 237 }, { 443, 237 }, { 444, 238 }, { 445, 238 }, { 446, 239 }, { 447, 239 }, { 448, 240 }, { 449, 240 }, { 450, 241 }, { 451, 241 }, { 452, 242 }, { 453, 242 }, { 454, 243 }, { 455, 244 }, { 456, 244 }, { 457, 245 }, { 458, 245 }, { 459, 246 }, { 460, 246 }, { 461, 247 }, { 462, 247 }, { 463, 248 }, { 464, 248 }, { 465, 249 }, { 466, 249 }, { 467, 250 }, { 468, 250 }, { 469, 251 }, { 470, 252 }, { 471, 252 }, { 472, 253 }, { 473, 253 }, { 474, 254 }, { 475, 254 }, { 476, 255 }, { 477, 255 }, { 478, 256 }, { 479, 256 }, { 480, 257 }, { 481, 257 }, { 482, 258 }, { 483, 258 }, { 484, 259 }, { 485, 259 }, { 486, 260 }, { 487, 261 }, { 488, 261 }, { 489, 262 }, { 490, 262 }, { 491, 263 }, { 492, 263 }, { 493, 264 }, { 494, 264 }, { 495, 265 }, { 496, 265 }, { 497, 266 }, { 498, 266 }, { 499, 267 }, { 500, 267 }, { 501, 268 }, { 502, 269 }, { 503, 269 }, { 504, 270 }, { 505, 270 }, { 506, 271 }, { 507, 271 }, { 508, 272 }, { 509, 272 }, { 510, 273 }, { 511, 273 }, { 512, 274 }, { 513, 274 }, { 514, 275 }, { 515, 275 }, { 516, 276 }, { 517, 277 }, { 518, 277 }, { 519, 278 }, { 520, 278 }, { 521, 279 }, { 522, 279 }, { 523, 280 }, { 524, 280 }, { 525, 281 }, { 526, 281 }, { 527, 282 }, { 528, 282 }, { 529, 283 }, { 530, 283 }, { 531, 284 }, { 532, 285 }, { 533, 285 }, { 534, 286 }, { 535, 286 }, { 536, 287 }, { 537, 287 }, { 538, 288 }, { 539, 288 }, { 540, 289 }, { 541, 289 }, { 542, 290 }, { 543, 290 }, { 544, 291 }, { 545, 291 }, { 546, 292 }, { 547, 293 }, { 548, 293 }, { 549, 294 }, { 550, 294 }, { 551, 295 }, { 552, 295 }, { 553, 296 }, { 554, 296 }, { 555, 297 }, { 556, 297 }, { 557, 298 }, { 558, 298 }, { 559, 299 }, { 560, 299 }, { 561, 300 }, { 562, 301 }, { 563, 301 }, { 564, 302 } };
  result.lines.push_back({ 2, points_cluster_3 });

  std::vector< cv::Point > points_cluster_4 = { { 52, 292 }, { 53, 291 }, { 54, 291 }, { 55, 290 }, { 56, 290 }, { 57, 289 }, { 58, 289 }, { 59, 288 }, { 60, 288 }, { 61, 287 }, { 62, 287 }, { 63, 286 }, { 64, 286 }, { 65, 285 }, { 66, 285 }, { 67, 284 }, { 68, 284 }, { 69, 283 }, { 70, 283 }, { 71, 282 }, { 72, 282 }, { 73, 281 }, { 74, 281 }, { 75, 280 }, { 76, 280 }, { 77, 279 }, { 78, 279 }, { 79, 278 }, { 80, 278 }, { 81, 277 }, { 82, 277 }, { 83, 276 }, { 84, 276 } };
  result.lines.push_back({ 1, points_cluster_4 });

  std::vector< cv::Point > points_cluster_5 = { { 172, 236 }, { 173, 235 }, { 174, 235 }, { 175, 234 }, { 176, 234 }, { 177, 233 }, { 178, 232 }, { 179, 232 }, { 180, 231 }, { 181, 231 }, { 182, 230 }, { 183, 230 }, { 184, 229 }, { 185, 228 }, { 186, 228 }, { 187, 227 }, { 188, 227 }, { 189, 226 }, { 190, 226 }, { 191, 225 }, { 192, 224 }, { 193, 224 }, { 194, 223 }, { 195, 223 }, { 196, 222 }, { 197, 222 }, { 198, 221 }, { 199, 220 }, { 200, 220 }, { 201, 219 }, { 202, 219 }, { 203, 218 }, { 204, 218 }, { 205, 217 }, { 206, 216 }, { 207, 216 }, { 208, 215 }, { 209, 215 }, { 210, 214 }, { 211, 214 }, { 212, 213 } };
  result.lines.push_back({ 1, points_cluster_5 });

  output_result(result);
  return 0 ;
#endif
}